#!/home/everdrone/Desktop/projects/jinfo/venv/bin/python

import os
import glob
import yaml
import re
import sys
import getopt
import json
from humanfriendly import parse_size

READ_AVAILABLE_FREQUENCIES = False


def read_from_file(path):
    with open(path, 'r') as f:
        return f.read()


def get_model_name():
    return read_from_file('/proc/device-tree/model').replace('\x00', '')


def get_soc_family():
    soc_family_path = '/proc/device-tree/compatible'
    soc_family_string = read_from_file(soc_family_path).replace('\x00', '')
    if 'tegra186' in soc_family_string:
        return 'tegra186'
    elif 'tegra194' in soc_family_string:
        return 'tegra194'
    elif 'tegra210' in soc_family_string:
        return 'tegra210'
    else:
        return False


def get_thermal_zones():
    thermal_zones_path = '/sys/devices/virtual/thermal/thermal_zone*'
    thermal_zones = glob.glob(thermal_zones_path)
    average = 0
    sensor_count = 0
    max_temp = 0
    temperatures = []
    for zone in thermal_zones:
        temp = int(read_from_file(os.path.join(zone, 'temp')).strip())
        name = read_from_file(os.path.join(zone, 'type')).strip()
        if 'PMIC' not in name:
            max_temp = max(max_temp, temp)
            sensor_count += 1
            average += temp
        temperatures.append({
            'name': name,
            'temp': temp / 1000
        })

    average = average / sensor_count if sensor_count > 0 else 0

    return {
        'sensors': temperatures,
        'max': max_temp / 1000,
        'average': average / 1000
    }


def get_cpu():
    # TODO: get name and stats from /proc/cpuinfo
    cpu_path = '/sys/devices/system/cpu/cpu[0-9]*'
    cpu_glob = glob.glob(cpu_path)
    cpus = {}

    for cpu in cpu_glob:
        if READ_AVAILABLE_FREQUENCIES:
            available_frequencies = []
            for freq in read_from_file(os.path.join(cpu, 'cpufreq/scaling_available_frequencies')).split(' '):
                if freq.strip():
                    available_frequencies.append(int(freq))

        idle_states_path = os.path.join(cpu, 'cpuidle/state[0-9]')
        idle_states = glob.glob(idle_states_path)
        states = []
        for idle_state in idle_states:
            state = {}
            state_name = read_from_file(
                os.path.join(idle_state, 'name')).strip()
            state_disable = int(read_from_file(
                os.path.join(idle_state, 'disable')))
            state_disable = True if state_disable == 1 else False

            state = {
                'name': state_name,
                'disable': state_disable
            }

            states.append(state)

        cpu_name = os.path.basename(os.path.normpath(cpu))
        cpus[cpu_name] = {
            'scaling': {
                'min_freq': int(read_from_file(os.path.join(cpu, 'cpufreq/scaling_min_freq'))),
                'max_freq': int(read_from_file(os.path.join(cpu, 'cpufreq/scaling_max_freq'))),
                'cur_freq': int(read_from_file(os.path.join(cpu, 'cpufreq/scaling_cur_freq'))),
            },
            'cpuinfo': {
                'min_freq': int(read_from_file(os.path.join(cpu, 'cpufreq/cpuinfo_min_freq'))),
                'max_freq': int(read_from_file(os.path.join(cpu, 'cpufreq/cpuinfo_max_freq'))),
                'cur_freq': int(read_from_file(os.path.join(cpu, 'cpufreq/cpuinfo_cur_freq'))),
            },
            'cpuidle': states
        }

        if READ_AVAILABLE_FREQUENCIES:
            cpus[cpu_name]['scaling']['available_frequencies'] = available_frequencies

    result_sorted = {}
    for cpu in sorted(cpus.keys()):
        result_sorted[cpu] = cpus[cpu]

    return result_sorted


def get_iram_clients():
    iram_path = '/sys/kernel/debug/nvmap/iram/clients'
    iram_lines = read_from_file(iram_path).splitlines()

    iram_regex = r'(\w+)(?:\s*)([a-zA-Z0-9\-\_\.]+)(?:\s*)(\-?\d+)(?:\s*)(\-?\d+\w+)'
    total_regex = r'(\w+)(?:\s*)(\-?\d+\w+)'

    processes = []

    for line in iram_lines[1:-1]:
        match = re.match(iram_regex, line)
        if match:
            processes.append({
                'client': match.group(1),
                'process': match.group(2),
                'pid': int(match.group(3)),
                'size': parse_size(match.group(4))
            })

    match = re.match(total_regex, iram_lines[-1])
    if match:
        output = {
            'total': parse_size(match.group(2)),
            'processes': processes
        }

    return output


def get_gpu():
    gpu_path = '/sys/class/devfreq/*'
    gpu_glob = glob.glob(gpu_path)

    gpus = {}

    for gpu in gpu_glob:
        gpu_name_path = os.path.join(gpu, 'device/of_node/name')

        if os.path.isfile(gpu_name_path):
            gpu_name = read_from_file(gpu_name_path)  # null terminating
            gpu_name = gpu_name.replace('\x00', '')

            if gpu_name in ['gp10b', 'gv11b', 'gpu']:
                # read railgate state
                railgate_enable = int(read_from_file(
                    os.path.join(gpu, 'device/railgate_enable')))
                railgate_enable = True if railgate_enable == 1 else False

                if READ_AVAILABLE_FREQUENCIES:
                    available_frequencies = []
                    for freq in read_from_file(os.path.join(gpu, 'available_frequencies')).split(' '):
                        if freq:
                            available_frequencies.append(int(freq))

                gpus[gpu_name] = {
                    'min_freq': int(read_from_file(os.path.join(gpu, 'min_freq'))),
                    'max_freq': int(read_from_file(os.path.join(gpu, 'max_freq'))),
                    'cur_freq': int(read_from_file(os.path.join(gpu, 'cur_freq'))),
                    'load': int(read_from_file(os.path.join(gpu, 'device/load'))),
                    'railgate_enable': railgate_enable,
                }

                if READ_AVAILABLE_FREQUENCIES:
                    gpus[gpu_name]['available_frequencies'] = available_frequencies
    return gpus


def get_fan():
    fan_path = '/sys/devices/pwm-fan'

    tach_enable = int(read_from_file(os.path.join(fan_path, 'tach_enable')))
    tach_enable = True if tach_enable == 1 else False

    temp_control = int(read_from_file(os.path.join(fan_path, 'temp_control')))
    temp_control = True if temp_control == 1 else False

    return {
        'tach_enable': tach_enable,
        'rpm_measured': int(read_from_file(os.path.join(fan_path, 'rpm_measured'))),
        'temp_control': temp_control,
        'pwm_cap': int(read_from_file(os.path.join(fan_path, 'pwm_cap'))),
        'cur_pwm': int(read_from_file(os.path.join(fan_path, 'cur_pwm'))),
        'target_pwm': int(read_from_file(os.path.join(fan_path, 'target_pwm'))),
    }


def get_emc():
    output = {}
    if get_soc_family() == 'tegra210':
        clk_override = int(read_from_file(
            '/sys/kernel/debug/clk/override.emc/clk_state'))
        clk_override = True if clk_override == 1 else False

        update_rate = int(read_from_file(
            '/sys/kernel/debug/clk/override.emc/clk_update_rate'))
        update_rate = True if update_rate == 1 else False

        output = {
            'min_freq': int(read_from_file('/sys/kernel/debug/tegra_bwmgr/emc_min_rate')),
            'max_freq': int(read_from_file('/sys/kernel/debug/tegra_bwmgr/emc_max_rate')),
            'cur_freq': int(read_from_file('/sys/kernel/debug/clk/override.emc/clk_rate')),
            'update_rate': update_rate,
            'clk_override': clk_override,
        }

    return output


use_json = False


def main():
    global use_json
    extra_argv = sys.argv[1:]
    opts, args = getopt.getopt(extra_argv, "jsmgcfte")

    output = {}

    def set_all():
        output['soc'] = get_soc_family()
        output['model'] = get_model_name()
        output['gpu'] = {
            'devices': get_gpu(),
            'clients': get_iram_clients()
        }
        output['cpu'] = get_cpu()
        output['fan'] = get_fan()
        output['thermal'] = get_thermal_zones()
        output['emc'] = get_emc()

    def cycle_all():
        global use_json
        for opt, arg in opts:
            if opt in ['-j']:
                use_json = True
            if opt in ['-s']:
                output['soc'] = get_soc_family()
            elif opt in ['-m']:
                output['model'] = get_model_name()
            elif opt in ['-g']:
                output['gpu'] = {
                    'devices': get_gpu(),
                    'clients': get_iram_clients()
                }
            elif opt in ['-c']:
                output['cpu'] = get_cpu()
            elif opt in ['-f']:
                output['fan'] = get_fan()
            elif opt in ['-t']:
                output['thermal'] = get_thermal_zones()
            elif opt in ['-e']:
                output['emc'] = get_emc()

    def cycle_opts():
        for opt, arg in opts:
            if opt in ['-s']:
                output['soc'] = get_soc_family()
            elif opt in ['-m']:
                output['model'] = get_model_name()
            elif opt in ['-g']:
                output['gpu'] = {
                    'devices': get_gpu(),
                    'clients': get_iram_clients()
                }
            elif opt in ['-c']:
                output['cpu'] = get_cpu()
            elif opt in ['-f']:
                output['fan'] = get_fan()
            elif opt in ['-t']:
                output['thermal'] = get_thermal_zones()
            elif opt in ['-e']:
                output['emc'] = get_emc()

    if len(opts) == 1:
        for opt, arg in opts:
            if opt in ['-j']:
                use_json = True
                set_all()
            else:
                cycle_opts()
    elif len(opts) == 0:
        set_all()
    else:
        cycle_all()

    if use_json:
        print(json.dumps(output, indent=2, sort_keys=True))
    else:
        print(yaml.dump(output, sort_keys=False))


if __name__ == '__main__':
    main()
