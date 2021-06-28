#pragma once

#include <glob.h>

#include <string>
#include <vector>

#include "defines.h"
#include "formatter.h"
#include "utils.h"

using std::string;
using std::vector;

typedef struct {
  string rail_name;
  unsigned crit_current_limit;
  unsigned crit_power_limit;
  unsigned in_current_input;
  unsigned in_current_trigger_input;
  unsigned in_power_input;
  unsigned in_power_trigger_input;
  unsigned in_voltage_input;
  unsigned warn_current_limit;
} device_power_t;

typedef struct {
  unsigned crit_current_limit_sum;
  vector<device_power_t> devices;
} power_t;

power_t get_power() {
  power_t result;

  glob_t glob_result;
  glob(POWER_INPUT_NAME_GLOB, GLOB_TILDE, NULL, &glob_result);
  for (unsigned i = 0; i < glob_result.gl_pathc; i++) {
    device_power_t device;

    string str;
    size_t cut_index = 0;

    str = POWER_INPUT_PATH "/rail_name_" + std::to_string(i);
    str = read_file(str.c_str());
    device.rail_name = trim(str);

    str = POWER_INPUT_PATH "/crit_current_limit_" + std::to_string(i);
    str = read_file(str.c_str());
    cut_index = str.find_first_of(' ');
    device.crit_current_limit = std::atoi(str.substr(0, cut_index).c_str());

    str = POWER_INPUT_PATH "/crit_power_limit_" + std::to_string(i);
    str = read_file(str.c_str());
    cut_index = str.find_first_of(' ');
    device.crit_power_limit = std::atoi(str.substr(0, cut_index).c_str());

    str = POWER_INPUT_PATH "/in_current" + std::to_string(i) + "_input";
    str = read_file(str.c_str());
    device.in_current_input = std::atoi(str.c_str());

    str = POWER_INPUT_PATH "/in_current" + std::to_string(i) + "_trigger_input";
    str = read_file(str.c_str());
    device.in_current_trigger_input = std::atoi(str.c_str());

    str = POWER_INPUT_PATH "/in_power" + std::to_string(i) + "_input";
    str = read_file(str.c_str());
    device.in_power_input = std::atoi(str.c_str());

    str = POWER_INPUT_PATH "/in_power" + std::to_string(i) + "_trigger_input";
    str = read_file(str.c_str());
    device.in_power_trigger_input = std::atoi(str.c_str());

    str = POWER_INPUT_PATH "/in_voltage" + std::to_string(i) + "_input";
    str = read_file(str.c_str());
    device.in_voltage_input = std::atoi(str.c_str());

    str = POWER_INPUT_PATH "/warn_current_limit_" + std::to_string(i);
    str = read_file(str.c_str());
    cut_index = str.find_first_of(' ');
    device.warn_current_limit = std::atoi(str.substr(0, cut_index).c_str());

    result.devices.push_back(device);
  }

  globfree(&glob_result);
  return result;
}

void pretty_print(power_t info, bool summary = false) {
  fmt_measure_t fmt;
  pretty("Power", "", 0);

  if (summary) {
    unsigned power_input_sum = 0;
    for (const auto& device : info.devices) {
      power_input_sum += device.in_power_input;
    }
    pretty("Total", std::to_string(power_input_sum).c_str(), 1, NUMBER, 0, "mW");
  } else {
    for (const auto& device : info.devices) {
      pretty("Name", device.rail_name.c_str(), 2, STRING, 18, "", true);
      pretty("Critical Current Limit", std::to_string(device.crit_current_limit).c_str(), 2, NUMBER,
             0, "mA");
      pretty("Critical Power Limit", std::to_string(device.crit_power_limit).c_str(), 2, NUMBER, 2,
             "mW");
      pretty("Current Input", std::to_string(device.in_current_input).c_str(), 2, NUMBER, 9, "mA");
      pretty("Current Trigger Input", std::to_string(device.in_current_trigger_input).c_str(), 2,
             NUMBER, 1, "mA");
      pretty("Power Input", std::to_string(device.in_power_input).c_str(), 2, NUMBER, 11, "mW");
      pretty("Power Trigger Input", std::to_string(device.in_power_trigger_input).c_str(), 2,
             NUMBER, 3, "mW");
      pretty("Voltage Input", std::to_string(device.in_voltage_input).c_str(), 2, NUMBER, 9, "mV");
    }
  }
}
