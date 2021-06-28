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
  Average: 31.9 C
  Max: 36.0 C
CPU:
  Avg Freq: 1.479 GHz
  Avg Speed: 100%
GPU:
  Freq: 921.600 MHz
  Speed: 100%
Memory:
  RAM: 33%
  Swap: 0%
  NVMap: 0%
Power:
  Total: 2938 mW
```

```yml
# sudo jnfo
Thermal:
  Average: 33.4 C
  Max: 38.0 C
  Sensors:
    - Name: AO
      Temp: 38.0 C
    - Name: CPU
      Temp: 35.0 C
    - Name: GPU
      Temp: 31.5 C
    - Name: PLL
      Temp: 29.5 C
    - Name: PMIC
      Temp: 100.0 C
    - Name: thermal
      Temp: 32.8 C
CPU:
  Avg Freq: 1.479 GHz
  Avg Speed: 100%
  Cores:
    - Name: cpu0
      Online: true
      Speed: 100%
      CPUInfo Freq:
        Min: 102.000 MHz
        Max: 1.479 GHz
        Cur: 1.479 GHz
      Scaling Freq:
        Min: 102.000 MHz
        Max: 1.479 GHz
        Cur: 1.479 GHz
      Idle State:
        - Name: WFI
          Disable: false
        - Name: c7
          Disable: false
    - Name: cpu1
      Online: true
      Speed: 100%
      CPUInfo Freq:
        Min: 102.000 MHz
        Max: 1.479 GHz
        Cur: 1.479 GHz
      Scaling Freq:
        Min: 102.000 MHz
        Max: 1.479 GHz
        Cur: 1.479 GHz
      Idle State:
        - Name: WFI
          Disable: false
        - Name: c7
          Disable: false
    - Name: cpu2
      Online: true
      Speed: 100%
      CPUInfo Freq:
        Min: 102.000 MHz
        Max: 1.479 GHz
        Cur: 1.479 GHz
      Scaling Freq:
        Min: 102.000 MHz
        Max: 1.479 GHz
        Cur: 1.479 GHz
      Idle State:
        - Name: WFI
          Disable: false
        - Name: c7
          Disable: false
    - Name: cpu3
      Online: true
      Speed: 100%
      CPUInfo Freq:
        Min: 102.000 MHz
        Max: 1.479 GHz
        Cur: 1.479 GHz
      Scaling Freq:
        Min: 102.000 MHz
        Max: 1.479 GHz
        Cur: 1.479 GHz
      Idle State:
        - Name: WFI
          Disable: false
        - Name: c7
          Disable: false
GPU:
  Load: 0
  Railgate: false
  Speed: 8%
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
    Total: 4.06 GB
    Free: 2.73 GB
    Cached: 663.71 MB
    Usage: 33%
  Swap:
    Total: 2.03 GB
    Free: 2.03 GB
    Cached: 0 B
    Usage: 0%
  NVMap:
    Total: 258.05 kB
    Free: 258.05 kB
    Used: 0 B
    Usage: 0%
Power:
  - Name: POM_5V_IN
    Critical Current Limit: 32760 mA
    Critical Power Limit: 163013 mW
    Current Input: 432 mA
    Current Trigger Input: 432 mA
    Power Input: 2146 mW
    Power Trigger Input: 2146 mW
    Voltage Input: 4968 mV
  - Name: POM_5V_GPU
    Critical Current Limit: 32760 mA
    Critical Power Limit: 162751 mW
    Current Input: 0 mA
    Current Trigger Input: 0 mA
    Power Input: 0 mW
    Power Trigger Input: 0 mW
    Voltage Input: 4968 mV
  - Name: POM_5V_CPU
    Critical Current Limit: 32760 mA
    Critical Power Limit: 162751 mW
    Current Input: 160 mA
    Current Trigger Input: 160 mA
    Power Input: 794 mW
    Power Trigger Input: 755 mW
    Voltage Input: 4968 mV
```
