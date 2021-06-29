#include <getopt.h>
#include <glob.h>

#include <iostream>

#include "config.h"
#include "cpu.h"
#include "defines.h"
#include "filesystem.h"
#include "formatter.h"
#include "gpu.h"
#include "mem.h"
#include "parse_size.h"
#include "power.h"
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
  power_t power;

  memory_t memory;
  vector<fs_t> filesystem;

  // TODO: ape_t ape
  // TODO: vic_t vic
  // TODO: emc_t emc
  vector<cpu_t> cpu;
  gpu_t gpu;
} jetson_info_t;

typedef struct options_struct {
  bool help = false;
  bool version = false;
  bool summary = false;
  bool all = false;
  bool cpu = false;
  bool gpu = false;
  bool thermal = false;
  bool memory = false;
  bool storage = false;
  bool power = false;
} options_t;

void print_version_exit() {
  std::cout << PACKAGE_STRING << std::endl;
  exit(EXIT_SUCCESS);
}

void print_help_exit() {
  // clang-format off
  printf(
    "%s [options]\n"
    "    -h --help         Show this help\n"
    "    -v --version      Show the program version\n"
    "    -s --summary      Less verbose\n"
    "    -c --cpu          Show CPU and cores information\n"
    "    -g --gpu          Show GPU information and clients\n"
    "    -t --thermal      Show thermal sensors\n"
    "    -m --memory       Show RAM, swap and NVMap\n"
    "    -f --storage      Show filesystems\n"
    "    -p --power        Show power and current information\n"
    "    -C --color        Enable colored output\n",
    progname);
  // clang-format on
  exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[]) {
  // set global program name
  progname = argv[0];

  /*
   * Parse options
   */
  options_t oobj;

  // clang-format off
  static const struct option long_options[] = {
    {"help",            no_argument,        NULL, 'h'},
    {"version",         no_argument,        NULL, 'v'},
    {"summary",         no_argument,        NULL, 's'},
    {"cpu",             no_argument,        NULL, 'c'},
    {"gpu",             no_argument,        NULL, 'g'},
    {"thermal",         no_argument,        NULL, 't'},
    {"memory",          no_argument,        NULL, 'm'},
    {"storage",         no_argument,        NULL, 'f'},
    {"power",           no_argument,        NULL, 'p'},
    {"color",           no_argument,        NULL, 'C'},
    {NULL,              0,                  NULL, 0}};
  // clang-format on

  int opt;
  while ((opt = getopt_long(argc, argv, "hvscgtmfpC", long_options, NULL)) >= 0) {
    switch (opt) {
      case 'h':
        oobj.help = true;
        break;
      case 'v':
        oobj.version = true;
        break;
      case 's':
        oobj.summary = true;
        break;
      case 'a':
        oobj.all = true;
        break;
      case 'c':
        oobj.cpu = true;
        break;
      case 'g':
        oobj.gpu = true;
        break;
      case 't':
        oobj.thermal = true;
        break;
      case 'm':
        oobj.memory = true;
        break;
      case 'f':
        oobj.storage = true;
        break;
      case 'p':
        oobj.power = true;
        break;
      case 'C':
        enable_color = true;
        break;
      default:
        return -1;
    }
  }

  if (optind < argc) {
    std::cerr << progname << ": too many arguments" << std::endl;
  }

  // print help and exit
  if (oobj.help) {
    print_help_exit();
  }

  // print version and exit
  if (oobj.version) {
    print_version_exit();
  }

  jetson_info_t result;

  if ((oobj.gpu || oobj.cpu || oobj.thermal || oobj.memory || oobj.storage || oobj.power) ==
      false) {
    oobj.all = true;
  }

  if (oobj.cpu || oobj.gpu || oobj.memory || oobj.power || oobj.all) {
    if (!is_sudo_or_root()) {
      std::cerr << progname << ": this operation requires root privileges." << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  if (oobj.all || oobj.thermal) {
    result.thermal = get_thermal_zones();
    pretty_print(result.thermal, oobj.summary);
  }

  if (oobj.all || oobj.cpu) {
    result.cpu = get_cpus();
    pretty_print(result.cpu, oobj.summary);
  }

  if (oobj.all || oobj.gpu) {
    result.gpu = get_gpu();
    pretty_print(result.gpu, oobj.summary);
  }

  if (oobj.all || oobj.memory) {
    result.memory = get_memory();
    pretty_print(result.memory, oobj.summary);
  }

  if (oobj.all || oobj.storage) {
    result.filesystem = get_filesystem();
    pretty_print(result.filesystem, oobj.summary);
  }

  if (oobj.all || oobj.power) {
    result.power = get_power();
    pretty_print(result.power, oobj.summary);
  }

  return 0;
}
