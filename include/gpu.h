#pragma once

#include <glob.h>

#include <string>
#include <vector>

#include "defines.h"
#include "formatter.h"
#include "parse_size.h"
#include "utils.h"

using std::string;
using std::vector;

typedef struct {
  string client;
  string process;
  int pid;
  unsigned size;
} gpu_process_t;

typedef struct {
  string name;
  unsigned clk_rate;
} engine_t;

typedef struct {
  // string name;
  unsigned min;
  unsigned max;
  unsigned cur;
  string load;
  bool railgate;
  unsigned total_clients;
  vector<gpu_process_t> processes;
  vector<engine_t> engines;
} gpu_t;

gpu_t get_gpu() {
  gpu_t result;
  engine_t engine;

  glob_t glob_result;

  glob(GPU_GLOB, GLOB_TILDE, NULL, &glob_result);
  for (unsigned i = 0; i < glob_result.gl_pathc; i++) {
    string name = glob_result.gl_pathv[i];
    name += "/device/of_node/name";

    string temp_str;

    name = read_file(name.c_str());
    if (name.find("gpu") != string::npos) {
      temp_str = glob_result.gl_pathv[i];
      temp_str += "device/railgate_enable";

      temp_str = read_file(temp_str.c_str());
      result.railgate = std::atoi(temp_str.c_str()) > 0 ? true : false;

      temp_str = glob_result.gl_pathv[i];
      temp_str += "/min_freq";
      result.min = std::atoi(read_file(temp_str.c_str()).c_str());

      temp_str = glob_result.gl_pathv[i];
      temp_str += "/max_freq";
      result.max = std::atoi(read_file(temp_str.c_str()).c_str());

      temp_str = glob_result.gl_pathv[i];
      temp_str += "/cur_freq";
      result.cur = std::atoi(read_file(temp_str.c_str()).c_str());

      // convert Hz to kHz
      result.min /= 1000;
      result.max /= 1000;
      result.cur /= 1000;

      temp_str = glob_result.gl_pathv[i];
      temp_str += "/device/load";
      temp_str = read_file(temp_str.c_str());
      result.load = trim(temp_str);

      /* parse clients */
      temp_str = read_file(GPU_CLIENTS_PATH);
      vector<string> lines = split_string(temp_str, "\n");
      temp_str.clear();

      size_t start = 0;
      size_t end = 0;
      size_t idx = 0;
      for (unsigned i = 1; i < lines.size(); i++) {
        end = lines[i].find_first_of(' ');
        if (lines[i].substr(0, end).find("total") != string::npos) {
          // total line
          start = lines[i].find_first_not_of(' ', end);
          result.total_clients = parse_size(lines[i].substr(start, lines[i].size() - 1));
        } else {
          if (lines[i].size() > 0) {
            // client
            gpu_process_t proc;
            proc.client = lines[i].substr(0, end);

            start = lines[i].find_first_not_of(' ', end);
            // end =
            //     lines[i].substr(start, lines[i].size() - 1).find_first_of('
            //     ') + start;
            end = lines[i].find_first_of(' ', start);
            proc.process = lines[i].substr(start, end - start);

            start = lines[i].find_first_not_of(' ', end);
            end = lines[i].find_first_of(' ', start);
            proc.pid = std::atoi(lines[i].substr(start, end).c_str());

            start = lines[i].find_first_not_of(' ', end);
            proc.size = parse_size(lines[i].substr(start, lines[i].size() - 1));

            result.processes.push_back(proc);
          }
        }
      }
      /* end parse */
    }
  }
  /* engines */
  engine.name = "NVDEC";
  engine.clk_rate = std::atoi(read_file(NVDEC_PATH).c_str());
  result.engines.push_back(engine);

  engine.name = "NVENC";
  engine.clk_rate = std::atoi(read_file(NVENC_PATH).c_str());
  result.engines.push_back(engine);

  engine.name = "NVJPG";
  engine.clk_rate = std::atoi(read_file(NVJPG_PATH).c_str());
  result.engines.push_back(engine);

  globfree(&glob_result);
  return result;
}

vector<engine_t> get_engines() {
  vector<engine_t> result;

  return result;
}

void pretty_print(gpu_t info, bool summary = false) {
  fmt_measure_t fmt;

  pretty("GPU", "", 0);
  if (summary) {
    fmt = format_frequency(info.cur * 1e+3, 3);
    pretty("Freq", fmt.value.c_str(), 1, NUMBER, 1, fmt.unit.c_str());
    pretty("Speed", format_percent(info.cur, info.max, 0).c_str(), 1, PERCENT, 0);
  } else {
    pretty("Load", info.load.c_str(), 1, NUMBER, 4);
    pretty("Railgate", info.railgate ? "true" : "false", 1, BOOL);
    pretty("Speed", format_percent(info.cur, info.max, 0).c_str(), 1, PERCENT, 3);
    pretty("Freq", "", 1);

    fmt = format_frequency(info.min * 1e+3, 3);
    pretty("Min", fmt.value.c_str(), 2, NUMBER, 0, fmt.unit.c_str());

    fmt = format_frequency(info.max * 1e+3, 3);
    pretty("Max", fmt.value.c_str(), 2, NUMBER, 0, fmt.unit.c_str());

    fmt = format_frequency(info.cur * 1e+3, 3);
    pretty("Cur", fmt.value.c_str(), 2, NUMBER, 0, fmt.unit.c_str());

    pretty("Engines", "", 1);
    for (const auto& engine : info.engines) {
      fmt = format_frequency(engine.clk_rate, 3);
      pretty(engine.name.c_str(), fmt.value.c_str(), 2, NUMBER, 0, fmt.unit.c_str());
    }

    pretty("Processes", "", 1);
    for (const auto& proc : info.processes) {
      pretty("Process", proc.process.c_str(), 3, STRING, 0, "", true);
      pretty("Client", proc.client.c_str(), 3, STRING, 1);
      pretty("PID", std::to_string(proc.pid).c_str(), 3, NUMBER, 4);
      fmt = format_storage(proc.size, 2);
      pretty("Size", fmt.value.c_str(), 3, NUMBER, 3, fmt.unit.c_str());
    }
    fmt = format_storage(info.total_clients, 2);
    pretty("Clients Total", fmt.value.c_str(), 1, NUMBER, 0, fmt.unit.c_str());
  }
}
