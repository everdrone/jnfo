#pragma once

#include <string>

#include "utils.h"

using std::string;

typedef struct {
  string value;
  string unit;
} fmt_measure_t;

fmt_measure_t format_frequency(unsigned hertz, int decimals) {
  fmt_measure_t out;
  char buffer[16];
  float result = float(hertz);

  if (hertz * 1e-9 >= 1) {
    result = float(hertz) * 1e-9;
    out.unit = "GHz";

  } else if (hertz * 1e-6 >= 1) {
    result = float(hertz) * 1e-6;
    out.unit = "MHz";

  } else if (hertz * 1e-3 >= 1) {
    result = float(hertz) * 1e-3;
    out.unit = "kHz";

  } else {
    // return hertz
    sprintf(buffer, "%d", hertz);
    out.unit = "Hz";
    out.value = buffer;
    return out;
  }

  sprintf(buffer, "%.*f", decimals, result);
  out.value = buffer;
  return out;
}

fmt_measure_t format_storage(unsigned bytes, int decimals,
                             bool use_iec = false) {
  fmt_measure_t out;
  char buffer[16];
  float result = float(bytes);

  if (use_iec && bytes / 1099511627776 >= 1) {
    // TiB
    result /= 1099511627776;  // 1024**4
    out.unit = "TiB";
  } else if (bytes * 1e-12 >= 1) {
    result *= 1e-12;
    out.unit = "TB";
  } else if (use_iec && bytes / 1073741824 >= 1) {
    result /= 1073741824;  // 1024**3
    out.unit = "GiB";
  } else if (bytes * 1e-9 >= 1) {
    result *= 1e-9;
    out.unit = "GB";
  } else if (use_iec && bytes / 1048576 >= 1) {
    result /= 1048576;  // 1024**2
    out.unit = "MiB";
  } else if (bytes * 1e-6 >= 1) {
    result *= 1e-6;
    out.unit = "MB";
  } else if (use_iec && bytes / 1024 >= 1) {
    result /= 1024;
    out.unit = "KiB";
  } else if (bytes * 1e-3 >= 1) {
    result *= 1e-3;
    out.unit = "kB";
  } else {
    // return bytes
    sprintf(buffer, "%d", bytes);
    out.value = buffer;
    out.unit = "B";
    return out;
  }

  sprintf(buffer, "%.*f", decimals, result);
  out.value = buffer;

  return out;
}

string format_percent(float x, float y, int decimals) {
  string out;
  char buffer[16];

  sprintf(buffer, "%.*f%%", decimals, x / y * 100.0);

  out = buffer;
  return out;
}
