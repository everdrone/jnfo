#pragma once

#if __APPLE__
#define TEST_PATH "/Users/giorgio.tropiano/projects/jetson/jnfo/test"
#else
#define TEST_PATH ""
#endif

#define MAX_NUMBER_FORMAT_SIZE = 16

// ANSI
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_BRIGHT_BLACK "\x1b[90m"
#define ANSI_COLOR_RESET "\x1b[0m"

// general
#define SOC_FAMILY_PATH TEST_PATH "/proc/device-tree/compatible"
#define MACHINE_NAME_PATH TEST_PATH "/proc/device-tree/model"
#define MEMINFO TEST_PATH "/proc/meminfo"

// thermal
#define THERMAL_ZONE_GLOB TEST_PATH "/sys/devices/virtual/thermal/thermal_zone*"

// cpu
#define CPU_GLOB TEST_PATH "/sys/devices/system/cpu/cpu[0-9]*"
#define CPU_IDLE_PARTIAL_GLOB "/cpuidle/state[0-9]"

// gpu
#define GPU_GLOB TEST_PATH "/sys/class/devfreq/*"
#define GPU_CLIENTS_PATH TEST_PATH "/sys/kernel/debug/nvmap/iram/clients"
#define NVDEC_PATH TEST_PATH "/sys/kernel/debug/clk/nvdec/clk_rate"
#define NVENC_PATH TEST_PATH "/sys/kernel/debug/clk/nvenc/clk_rate"
#define NVJPG_PATH TEST_PATH "/sys/kernel/debug/clk/nvjpg/clk_rate"

// memory
#define IRAM_SIZE_PATH TEST_PATH "/sys/kernel/debug/nvmap/iram/size"

// filesystem
#define MTAB_PATH TEST_PATH "/etc/mtab"

// power
#define POWER_INPUT_NAME_GLOB \
  TEST_PATH "/sys/bus/i2c/drivers/ina3221x/6-0040/iio:device0/rail_name_[0-9]"
#define POWER_INPUT_PATH TEST_PATH "/sys/bus/i2c/drivers/ina3221x/6-0040/iio:device0"

static char* progname = "jnfo";
static bool enable_color = false;
