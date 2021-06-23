#include <glob.h>

#include <iostream>

#include "cpu.h"
#include "defines.h"
#include "gpu.h"
#include "mem.h"
#include "parse_size.h"
#include "thermal.h"
#include "utils.h"

string get_soc_family() {
  string soc_family = read_file(SOC_FAMILY_PATH);

  if (soc_family.find("tegra186") != string::npos) {
    soc_family = "tegra186";
  } else if (soc_family.find("tegra210") != string::npos) {
    soc_family = "tegra210";
  } else if (soc_family.find("tegra194") != string::npos) {
    soc_family = "tegra194";
  } else {
    std::cerr << "Model `" << soc_family << "' is not supported." << std::endl;
    exit(EXIT_FAILURE);
  }

  return soc_family;
}

string get_model_name() {
  return read_file(MACHINE_NAME_PATH);
}

typedef struct {
  string soc_family;
  string model_name;

  thermal_t thermal;

  memory_t memory;

  vector<cpu_t> cpu;
  gpu_t gpu;
} jetson_info_t;

// TODO add option to display raw values
char* millicelsius_to_celsius(unsigned millicelsius) {
  float float_temp = float(millicelsius) / 1000;
  char buffer[16];
  sprintf(buffer, "%.2f C", float_temp);

  return buffer;
}

char* hertz_to_megahertz(unsigned hertz) {
  float float_temp = float(hertz) / 1000000;
  char buffer[16];
  sprintf(buffer, "%.2f MHz", float_temp);

  return buffer;
}

void pretty_print(jetson_info_t info) {
  /* thernal */
  pretty("Thermal", "", 0);
  pretty("Average", std::to_string(info.thermal.average).c_str(), 1);
  pretty("Max", std::to_string(info.thermal.max).c_str(), 1);

  pretty("Sensors", "", 1);
  for (const auto& sensor : info.thermal.sensors) {
    pretty("Name", sensor.name.c_str(), 2, false, true);
    pretty("Temp", std::to_string(sensor.temp).c_str(), 2);
  }

  /* cpu */
  pretty("CPU", "", 0);
  for (const auto& cpu : info.cpu) {
    // multiply hz by 1000
    pretty("Name", cpu.name.c_str(), 1, true, true);
    pretty("Online", cpu.online ? "true" : "false", 2);
    pretty("CPUInfo Freq", "", 2);
    pretty("Min", std::to_string(cpu.cpuinfo.min).c_str(), 3);
    pretty("Max", std::to_string(cpu.cpuinfo.max).c_str(), 3);
    pretty("Cur", std::to_string(cpu.cpuinfo.cur).c_str(), 3);
    pretty("Scaling Freq", "", 2);
    pretty("Min", std::to_string(cpu.scaling.min).c_str(), 3);
    pretty("Max", std::to_string(cpu.scaling.max).c_str(), 3);
    pretty("Cur", std::to_string(cpu.scaling.cur).c_str(), 3);

    pretty("Idle State", "", 2);
    for (const auto& cpuidle : cpu.cpuidle) {
      pretty("Name", cpuidle.name.c_str(), 3, true, true);
      pretty("Disable", cpuidle.disable ? "true" : "false", 3);
    }
  }

  /* gpu */
  pretty("GPU", "", 0);
  pretty("Load", info.gpu.load.c_str(), 1, false);
  pretty("Freq", "", 1);
  pretty("Min", std::to_string(info.gpu.min).c_str(), 2);
  pretty("Max", std::to_string(info.gpu.max).c_str(), 2);
  pretty("Cur", std::to_string(info.gpu.cur).c_str(), 2);
  pretty("Railgate", info.gpu.railgate ? "enabled" : "disabled", 1);

  /* memory */
  pretty("Memory", "", 0);
  pretty("RAM", "", 1);
  pretty("Total", std::to_string(info.memory.ram.total).c_str(), 2);
  pretty("Free", std::to_string(info.memory.ram.free).c_str(), 2);
  pretty("Cached", std::to_string(info.memory.ram.cached).c_str(), 2);
  pretty("Swap", "", 1);
  pretty("Total", std::to_string(info.memory.swap.total).c_str(), 2);
  pretty("Free", std::to_string(info.memory.swap.free).c_str(), 2);
  pretty("Cached", std::to_string(info.memory.swap.cached).c_str(), 2);
  pretty("NVMap", "", 1);
  pretty("Total", std::to_string(info.memory.nvmap.total).c_str(), 2);
  pretty("Free", std::to_string(info.memory.nvmap.free).c_str(), 2);
}

int main(int argc, char* argv[]) {
  jetson_info_t result;

  result.thermal = get_thermal_zones();
  result.cpu = get_cpus();
  result.gpu = get_gpu();
  result.memory = get_memory();

  pretty_print(result);

  return 0;
}
