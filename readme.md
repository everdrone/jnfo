<h1 align="center"><b>J</b>etson <b>N</b>ano In<b>fo</b></h1>

<p align="center">Prints informations relative to the NVIDIA Jetson Nano</p>

## Build & Install

#### Requirements

- Autotools
- Autoconf Archive (`apt install autoconf-archive`)

```sh
./autogen.sh && cd build

../configure

# installation requires privileges
sudo make install
```

## Usage

```
jnfo [options]
    -h --help         Show this help
    -v --version      Show the program version
    -s --summary      Less verbose
    -c --cpu          Show CPU and cores information
    -g --gpu          Show GPU information and clients
    -t --thermal      Show thermal sensors
    -m --memory       Show RAM, swap and NVMap
    -p --power        Show power and current information
    -C --color        Enable colored output
```

## Sample Output

```yml
# sudo jnfo -s
Thermal:
  Average: 31.7 C
  Max:     36.0 C
CPU:
  Avg Freq:  1.479 GHz
  Avg Speed: 100%
GPU:
  Freq:  76.800 MHz
  Speed: 8%
Memory:
  RAM:   33%
  Swap:  0%
  NVMap: 0%
Filesystem:
  Capacity: 125.72 GB
  Used:     11%
Power:
  Total: 2078 mW
```

```yml
# sudo jnfo
Thermal:
  Average: 31.6 C
  Max:     36.0 C
  Sensors:
    - Name: AO
      Temp: 36.0 C
    - Name: CPU
      Temp: 32.5 C
    - Name: GPU
      Temp: 30.5 C
    - Name: PLL
      Temp: 27.5 C
    - Name: PMIC
      Temp: 100.0 C
    - Name: thermal
      Temp: 31.5 C
CPU:
  Avg Freq:  1.479 GHz
  Avg Speed: 100%
  Cores:
    - Name:   cpu0
      Online: true
      Speed:  100%
      CPUInfo Freq:
        Min: 102.000 MHz
        Max: 1.479 GHz
        Cur: 1.479 GHz
      Scaling Freq:
        Min: 102.000 MHz
        Max: 1.479 GHz
        Cur: 1.479 GHz
      Idle State:
        - Name:    WFI
          Disable: false
        - Name:    c7
          Disable: false
    - Name:   cpu1
      Online: true
      Speed:  100%
      CPUInfo Freq:
        Min: 102.000 MHz
        Max: 1.479 GHz
        Cur: 1.479 GHz
      Scaling Freq:
        Min: 102.000 MHz
        Max: 1.479 GHz
        Cur: 1.479 GHz
      Idle State:
        - Name:    WFI
          Disable: false
        - Name:    c7
          Disable: false
    - Name:   cpu2
      Online: true
      Speed:  100%
      CPUInfo Freq:
        Min: 102.000 MHz
        Max: 1.479 GHz
        Cur: 1.479 GHz
      Scaling Freq:
        Min: 102.000 MHz
        Max: 1.479 GHz
        Cur: 1.479 GHz
      Idle State:
        - Name:    WFI
          Disable: false
        - Name:    c7
          Disable: false
    - Name:   cpu3
      Online: true
      Speed:  100%
      CPUInfo Freq:
        Min: 102.000 MHz
        Max: 1.479 GHz
        Cur: 1.479 GHz
      Scaling Freq:
        Min: 102.000 MHz
        Max: 1.479 GHz
        Cur: 1.479 GHz
      Idle State:
        - Name:    WFI
          Disable: false
        - Name:    c7
          Disable: false
GPU:
  Load:     0
  Railgate: false
  Speed:    8%
  Freq:
    Min: 76.800 MHz
    Max: 921.600 MHz
    Cur: 76.800 MHz
  Engines:
    NVDEC: 268.800 MHz
    NVENC: 268.800 MHz
    NVJPG: 192.000 MHz
  Processes:
  Clients Total: 0 B
Memory:
  RAM:
    Total:  4.06 GB
    Free:   2.73 GB
    Cached: 664.81 MB
    Usage:  33%
  Swap:
    Total:  2.03 GB
    Free:   2.03 GB
    Cached: 0 B
    Usage:  0%
  NVMap:
    Total:  258.05 kB
    Free:   258.05 kB
    Used:   0 B
    Usage:  0%
Filesystem:
  - Name:      /dev/mmcblk0p1
    Path:      /
    Capacity:  125.72 GB
    Free:      112.47 GB
    Available: 107.20 GB
    Used:      11%
  - Name:      proc
    Path:      /proc
    Capacity:  0 B
    Free:      0 B
    Available: 0 B
    Used:      0%
  - Name:      sysfs
    Path:      /sys
    Capacity:  0 B
    Free:      0 B
    Available: 0 B
    Used:      0%
  - Name:      none
    Path:      /dev
    Capacity:  1.83 GB
    Free:      1.83 GB
    Available: 1.83 GB
    Used:      0%
  - Name:      tmpfs
    Path:      /run
    Capacity:  2.08 GB
    Free:      2.06 GB
    Available: 2.06 GB
    Used:      1%
Power:
  - Name:                   POM_5V_IN
    Critical Current Limit: 32760 mA
    Critical Power Limit:   163800 mW
    Current Input:          312 mA
    Current Trigger Input:  320 mA
    Power Input:            1600 mW
    Power Trigger Input:    1600 mW
    Voltage Input:          5000 mV
  - Name:                   POM_5V_GPU
    Critical Current Limit: 32760 mA
    Critical Power Limit:   163537 mW
    Current Input:          0 mA
    Current Trigger Input:  0 mA
    Power Input:            0 mW
    Power Trigger Input:    0 mW
    Voltage Input:          4992 mV
  - Name:                   POM_5V_CPU
    Critical Current Limit: 32760 mA
    Critical Power Limit:   163537 mW
    Current Input:          104 mA
    Current Trigger Input:  112 mA
    Power Input:            558 mW
    Power Trigger Input:    558 mW
    Voltage Input:          4984 mV
```
