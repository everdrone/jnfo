#pragma once

// #define TEST_PATH "/home/everdrone/Desktop/projects/jnfo/test"
#define TEST_PATH ""

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
