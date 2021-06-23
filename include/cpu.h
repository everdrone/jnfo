#pragma once

#include <glob.h>

#include <string>
#include <vector>

#include "defines.h"
#include "utils.h"

using std::string;
using std::vector;

typedef struct {
  unsigned min;
  unsigned max;
  unsigned cur;
} cpu_freq_t;

typedef struct {
  string name;
  bool disable;
} cpu_idle_t;

typedef struct {
  string name;
  bool online;
  cpu_freq_t scaling;
  cpu_freq_t cpuinfo;
  vector<cpu_idle_t> cpuidle;
} cpu_t;

vector<cpu_t> get_cpus() {
  vector<cpu_t> result;

  glob_t glob_result;
  glob_t glob_cpuidle_result;

  glob(CPU_GLOB, GLOB_TILDE, NULL, &glob_result);
  for (unsigned i = 0; i < glob_result.gl_pathc; i++) {
    cpu_t cpu;

    string cpu_path = glob_result.gl_pathv[i];
    string cpuidle_glob = cpu_path + CPU_IDLE_PARTIAL_GLOB;

    // set cpuX name
    string name = "cpu";
    name += std::to_string(i);
    cpu.name = name;

    // get idle states
    glob(cpuidle_glob.c_str(), GLOB_TILDE, NULL, &glob_cpuidle_result);
    for (unsigned j = 0; j < glob_cpuidle_result.gl_pathc; j++) {
      cpu_idle_t cpuidle;

      name = glob_cpuidle_result.gl_pathv[j];
      name += "/name";
      string disable = glob_cpuidle_result.gl_pathv[j];
      disable += "/disable";

      cpuidle.name = read_file(name.c_str());
      cpuidle.name = trim(cpuidle.name);

      disable = read_file(disable.c_str());
      cpuidle.disable = std::atoi(disable.c_str()) > 0 ? true : false;

      cpu.cpuidle.push_back(cpuidle);
    }

    string temp_path;

    // online state

    temp_path = glob_result.gl_pathv[i];
    temp_path += "/online";
    cpu.online = std::atoi(read_file(temp_path.c_str()).c_str()) > 0 ? true : false;

    temp_path = glob_result.gl_pathv[i];
    temp_path += "/cpufreq/scaling_min_freq";
    cpu.scaling.min = std::atoi(read_file(temp_path.c_str()).c_str());

    temp_path = glob_result.gl_pathv[i];
    temp_path += "/cpufreq/scaling_max_freq";
    cpu.scaling.max = std::atoi(read_file(temp_path.c_str()).c_str());

    temp_path = glob_result.gl_pathv[i];
    temp_path += "/cpufreq/scaling_cur_freq";
    cpu.scaling.cur = std::atoi(read_file(temp_path.c_str()).c_str());

    temp_path = glob_result.gl_pathv[i];
    temp_path += "/cpufreq/cpuinfo_min_freq";
    cpu.cpuinfo.min = std::atoi(read_file(temp_path.c_str()).c_str());

    temp_path = glob_result.gl_pathv[i];
    temp_path += "/cpufreq/cpuinfo_max_freq";
    cpu.cpuinfo.max = std::atoi(read_file(temp_path.c_str()).c_str());

    temp_path = glob_result.gl_pathv[i];
    temp_path += "/cpufreq/cpuinfo_cur_freq";
    cpu.cpuinfo.cur = std::atoi(read_file(temp_path.c_str()).c_str());

    result.push_back(cpu);
  }

  globfree(&glob_cpuidle_result);
  globfree(&glob_result);
  return result;
}
