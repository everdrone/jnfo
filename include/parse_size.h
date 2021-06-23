#pragma once

#include <stdlib.h>

#include "utils.h"

unsigned parse_size(string str) {
  size_t pos = str.find_first_not_of("0123456789.,+- ");

  if (pos != string::npos) {
    // found a match
    char multiplier = str[pos];
    switch (multiplier) {
      case 'b':
      case 'B':
        return std::atof(str.substr(0, pos).c_str());
        break;
      case 'k':
      case 'K':
        return std::atof(str.substr(0, pos).c_str()) * 1e+3;
        break;
      case 'M':
        return std::atof(str.substr(0, pos).c_str()) * 1e+6;
        break;
      case 'G':
        return std::atof(str.substr(0, pos).c_str()) * 1e+9;
        break;
      case 'T':
        return std::atof(str.substr(0, pos).c_str()) * 1e+12;
        break;
      default:
        std::cerr << "cannot parse size: " << str << std::endl;
        std::cerr << "at position: " << pos << std::endl;
        exit(EXIT_FAILURE);
        break;
    }
  }
}
