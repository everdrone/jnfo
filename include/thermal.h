#pragma once

#include <glob.h>

#include <algorithm>
#include <string>
#include <vector>

#include "defines.h"
#include "utils.h"

using std::string;
using std::vector;

typedef struct {
  string name;
  unsigned temp;
} thermal_sensor_t;

typedef struct thermal_struct {
  vector<thermal_sensor_t> sensors;
  unsigned average = 0;
  unsigned max = 0;
} thermal_t;

// TODO: look into /sys/kernel/debug/soctherm
// for throttling and thermals

thermal_t get_thermal_zones() {
  thermal_t result;

  glob_t glob_result;

  unsigned num_sensors = 0;

  glob(THERMAL_ZONE_GLOB, GLOB_TILDE, NULL, &glob_result);
  for (unsigned i = 0; i < glob_result.gl_pathc; i++) {
    thermal_sensor_t sensor;

    string name = glob_result.gl_pathv[i];
    name += "/type";

    string temp = glob_result.gl_pathv[i];
    temp += "/temp";

    name = read_file(name.c_str());
    temp = read_file(temp.c_str());

    // find the first dash (-) and cut the name from there
    size_t dash_index = name.find_first_of('-');
    sensor.name = name.substr(0, dash_index);

    sensor.temp = std::atoi(temp.c_str());

    result.sensors.push_back(sensor);
    if (!name.find("PMIC")) {
      // ignore PMIC
      continue;
    } else {
      num_sensors++;

      result.max = std::max(result.max, sensor.temp);
      result.average += sensor.temp;
    }
  }

  // get the average
  if (num_sensors > 0) {
    result.average /= num_sensors;
  }

  globfree(&glob_result);
  return result;
}

void pretty_print(thermal_t info, bool summary = false) {
  char buffer[10];

  pretty("Thermal", "", 0);
  sprintf(buffer, "%.1f", float(info.average) * 1e-3);
  pretty("Average", buffer, 1, NUMBER, 0, "C");

  sprintf(buffer, "%.1f", float(info.max) * 1e-3);
  pretty("Max", buffer, 1, NUMBER, 4, "C");

  if (!summary) {
    pretty("Sensors", "", 1);
    for (const auto& sensor : info.sensors) {
      pretty("Name", sensor.name.c_str(), 3, STRING, 0, "", true);

      sprintf(buffer, "%.1f", float(sensor.temp) * 1e-3);
      pretty("Temp", buffer, 3, NUMBER, 0, "C");
    }
  }
}
