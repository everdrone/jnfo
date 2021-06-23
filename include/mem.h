#pragma once

#include <string>

#include "defines.h"
#include "parse_size.h"
#include "utils.h"

using std::string;

// TODO: add cached?
typedef struct {
  unsigned total;
  unsigned free;
  unsigned cached;
} mem_section_t;

typedef struct {
  mem_section_t ram;
  mem_section_t swap;
  mem_section_t nvmap;
} memory_t;

memory_t get_memory() {
  memory_t result;

  // read /proc/meminfo line by line
  vector<string> meminfo = read_lines(MEMINFO);

  for (const auto& line : meminfo) {
    size_t cut_index = 0;

    if (line.find("MemTotal:") != std::string::npos) {
      // total mem
      cut_index = line.find(':');
      result.ram.total = parse_size(line.substr(cut_index + 1, line.size()));
    }

    if (line.substr(0, 9).find("MemFree:") != std::string::npos) {
      cut_index = line.find(':');
      result.ram.free = parse_size(line.substr(cut_index + 1, line.size()));
    }

    if (line.substr(0, 8).find("Cached:") != std::string::npos) {
      cut_index = line.find(':');
      result.ram.cached = parse_size(line.substr(cut_index + 1, line.size()));
    }

    if (line.find("SwapTotal:") != std::string::npos) {
      cut_index = line.find(':');
      result.swap.total = parse_size(line.substr(cut_index + 1, line.size()));
    }

    if (line.find("SwapFree:") != std::string::npos) {
      cut_index = line.find(':');
      result.swap.free = parse_size(line.substr(cut_index + 1, line.size()));
    }

    if (line.find("SwapCached:") != std::string::npos) {
      cut_index = line.find(':');
      result.swap.cached = parse_size(line.substr(cut_index + 1, line.size()));
    }

    if (line.find("NvMapMemFree:") != std::string::npos) {
      cut_index = line.find(':');
      result.nvmap.free = parse_size(line.substr(cut_index + 1, line.size()));
      result.nvmap.total += result.nvmap.free;
    }

    if (line.find("NvMapMemUsed:") != std::string::npos) {
      cut_index = line.find(':');
      result.nvmap.total = parse_size(line.substr(cut_index + 1, line.size()));
    }
  }

  return result;
}
