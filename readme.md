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
    -f --storage      Show filesystems
    -n --network      Show network interfaces
    -p --power        Show power and current information
    -C --color        Enable colored output
```

## Sample Output

```yml
# sudo jnfo -s
Thermal:
  Average: 32.8 C
  Max:     37.0 C
CPU:
  Avg Freq:  1.479 GHz
  Avg Speed: 100%
GPU:
  Freq:  921.600 MHz
  Speed: 100%
Memory:
  RAM:
    Total: 4.06 GB
    Usage: 21%
  Swap:
    Total: 2.03 GB
    Usage: 0%
  NVMap:
    Total: 258.05 kB
    Usage: 0%
Filesystem:
  Capacity: 125.72 GB
  Used:     11%
Network:
  Receive:  91.29 MB
  Transmit: 8.37 MB
Power:
  Total: 2978 mW
```

```yml
# sudo jnfo
Thermal:
  Average: 33.2 C
  Max:     37.5 C
  Sensors:
    - Name: AO
      Temp: 37.5 C
    - Name: CPU
      Temp: 34.0 C
    - Name: GPU
      Temp: 32.5 C
    - Name: PLL
      Temp: 29.0 C
    - Name: PMIC
      Temp: 100.0 C
    - Name: thermal
      Temp: 33.2 C
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
        Min: 1.479 GHz
        Max: 1.479 GHz
        Cur: 1.479 GHz
      Idle State:
        - Name:    WFI
          Disable: true
        - Name:    c7
          Disable: true
    - Name:   cpu1
      Online: true
      Speed:  100%
      CPUInfo Freq:
        Min: 102.000 MHz
        Max: 1.479 GHz
        Cur: 1.479 GHz
      Scaling Freq:
        Min: 1.479 GHz
        Max: 1.479 GHz
        Cur: 1.479 GHz
      Idle State:
        - Name:    WFI
          Disable: true
        - Name:    c7
          Disable: true
    - Name:   cpu2
      Online: true
      Speed:  100%
      CPUInfo Freq:
        Min: 102.000 MHz
        Max: 1.479 GHz
        Cur: 1.479 GHz
      Scaling Freq:
        Min: 1.479 GHz
        Max: 1.479 GHz
        Cur: 1.479 GHz
      Idle State:
        - Name:    WFI
          Disable: true
        - Name:    c7
          Disable: true
    - Name:   cpu3
      Online: true
      Speed:  100%
      CPUInfo Freq:
        Min: 102.000 MHz
        Max: 1.479 GHz
        Cur: 1.479 GHz
      Scaling Freq:
        Min: 1.479 GHz
        Max: 1.479 GHz
        Cur: 1.479 GHz
      Idle State:
        - Name:    WFI
          Disable: true
        - Name:    c7
          Disable: true
GPU:
  Load:     0
  Railgate: false
  Speed:    100%
  Freq:
    Min: 921.600 MHz
    Max: 921.600 MHz
    Cur: 921.600 MHz
  Engines:
    NVDEC: 268.800 MHz
    NVENC: 268.800 MHz
    NVJPG: 192.000 MHz
  Processes:
  Clients Total: 0 B
Memory:
  RAM:
    Total:  4.06 GB
    Free:   3.22 GB
    Cached: 592.04 MB
    Usage:  21%
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
Network:
  Receive:
    Bytes:   91.29 MB
    Packets: 137236
    Dropped: 0
    Errors:  0
  Transmit:
    Bytes:   8.35 MB
    Packets: 69540
    Dropped: 0
    Errors:  0
  Interfaces:
    - Name:    eth0
      Address: 00:04:4b:ea:e2:5d
      Status:  up
      Speed:   1.00 GB/s
      Receive:
        Bytes:   91.29 MB
        Packets: 137236
        Dropped: 0
        Errors:  0
      Transmit:
        Bytes:   8.35 MB
        Packets: 69540
        Dropped: 0
        Errors:  0
    - Name:    rndis0
      Address: da:1b:ea:9f:16:5d
      Status:  down
      Speed:   0 B/s
      Receive:
        Bytes:   0 B
        Packets: 0
        Dropped: 0
        Errors:  0
      Transmit:
        Bytes:   0 B
        Packets: 0
        Dropped: 0
        Errors:  0
    - Name:    usb0
      Address: da:1b:ea:9f:16:5f
      Status:  down
      Speed:   0 B/s
      Receive:
        Bytes:   0 B
        Packets: 0
        Dropped: 0
        Errors:  0
      Transmit:
        Bytes:   0 B
        Packets: 0
        Dropped: 0
        Errors:  0
Power:
  - Name:                   POM_5V_IN
    Critical Current Limit: 32760 mA
    Critical Power Limit:   161965 mW
    Current Input:          560 mA
    Current Trigger Input:  552 mA
    Power Input:            2729 mW
    Power Trigger Input:    2729 mW
    Voltage Input:          4944 mV
  - Name:                   POM_5V_GPU
    Critical Current Limit: 32760 mA
    Critical Power Limit:   161965 mW
    Current Input:          32 mA
    Current Trigger Input:  32 mA
    Power Input:            158 mW
    Power Trigger Input:    158 mW
    Voltage Input:          4944 mV
  - Name:                   POM_5V_CPU
    Critical Current Limit: 32760 mA
    Critical Power Limit:   161965 mW
    Current Input:          184 mA
    Current Trigger Input:  184 mA
    Power Input:            909 mW
    Power Trigger Input:    870 mW
    Voltage Input:          4944 mV
```
