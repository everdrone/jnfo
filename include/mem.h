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

  result.nvmap.total = std::strtoul(read_file(IRAM_SIZE_PATH).c_str(), NULL, 16);

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

    // if (line.find("NvMapMemFree:") != std::string::npos) {
    //   cut_index = line.find(':');
    //   result.nvmap.free = parse_size(line.substr(cut_index + 1,
    //   line.size()));
    // }

    if (line.find("NvMapMemUsed:") != std::string::npos) {
      cut_index = line.find(':');
      result.nvmap.cached = parse_size(line.substr(cut_index + 1, line.size()));
    }

    result.nvmap.free = result.nvmap.total - result.nvmap.cached;
  }

  return result;
}

void pretty_print(memory_t info, bool summary = false) {
  fmt_measure_t fmt;
  pretty("Memory", "", 0);

  if (summary) {
    pretty("RAM", "", 1);
    fmt = format_storage(info.ram.total, 2);
    pretty("Total", fmt.value.c_str(), 2, NUMBER, 0, fmt.unit.c_str());
    pretty("Usage", format_percent(info.ram.total - info.ram.free, info.ram.total, 0).c_str(), 2,
           PERCENT, 0);
    pretty("Swap", "", 1);
    fmt = format_storage(info.swap.total, 2);
    pretty("Total", fmt.value.c_str(), 2, NUMBER, 0, fmt.unit.c_str());
    pretty("Usage", format_percent(info.swap.total - info.swap.free, info.swap.total, 0).c_str(), 2,
           PERCENT, 0);
    pretty("NVMap", "", 1);
    fmt = format_storage(info.nvmap.total, 2);
    pretty("Total", fmt.value.c_str(), 2, NUMBER, 0, fmt.unit.c_str());
    pretty("Usage", format_percent(info.nvmap.cached, info.nvmap.total, 0).c_str(), 2, PERCENT, 0);
  } else {
    pretty("RAM", "", 1);

    fmt = format_storage(info.ram.total, 2);
    pretty("Total", fmt.value.c_str(), 2, NUMBER, 1, fmt.unit.c_str());

    fmt = format_storage(info.ram.free, 2);
    pretty("Free", fmt.value.c_str(), 2, NUMBER, 2, fmt.unit.c_str());

    fmt = format_storage(info.ram.cached, 2);
    pretty("Cached", fmt.value.c_str(), 2, NUMBER, 0, fmt.unit.c_str());
    pretty("Usage", format_percent(info.ram.total - info.ram.free, info.ram.total, 0).c_str(), 2,
           PERCENT, 1);
    pretty("Swap", "", 1);

    fmt = format_storage(info.swap.total, 2);
    pretty("Total", fmt.value.c_str(), 2, NUMBER, 1, fmt.unit.c_str());

    fmt = format_storage(info.swap.free, 2);
    pretty("Free", fmt.value.c_str(), 2, NUMBER, 2, fmt.unit.c_str());

    fmt = format_storage(info.swap.cached, 2);
    pretty("Cached", fmt.value.c_str(), 2, NUMBER, 0, fmt.unit.c_str());
    pretty("Usage", format_percent(info.swap.total - info.swap.free, info.swap.total, 0).c_str(), 2,
           PERCENT, 1);
    pretty("NVMap", "", 1);

    fmt = format_storage(info.nvmap.total, 2);
    pretty("Total", fmt.value.c_str(), 2, NUMBER, 1, fmt.unit.c_str());

    fmt = format_storage(info.nvmap.free, 2);
    pretty("Free", fmt.value.c_str(), 2, NUMBER, 2, fmt.unit.c_str());

    fmt = format_storage(info.nvmap.cached, 2);
    pretty("Used", fmt.value.c_str(), 2, NUMBER, 2, fmt.unit.c_str());
    pretty("Usage", format_percent(info.nvmap.cached, info.nvmap.total, 0).c_str(), 2, PERCENT, 1);
  }
}
