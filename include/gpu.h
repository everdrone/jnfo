#pragma once

#include <glob.h>

#include <string>
#include <vector>

#include "defines.h"
#include "utils.h"

using std::string;
using std::vector;

typedef struct {
  string client;
  string process;
  int pid;
  string size;
} gpu_process_t;

typedef struct {
  // string name;
  unsigned min;
  unsigned max;
  unsigned cur;
  string load;
  bool railgate;
  unsigned total_clients;
  vector<gpu_process_t> processes;
} gpu_t;

gpu_t get_gpu() {
  gpu_t result;

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

      temp_str = glob_result.gl_pathv[i];
      temp_str += "/device/load";
      result.load = read_file(temp_str.c_str());

      // TODO: add gpu clients
    }
  }

  globfree(&glob_result);
  return result;
}
