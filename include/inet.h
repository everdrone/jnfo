#pragma once

#include <glob.h>
#include <libgen.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "defines.h"
#include "utils.h"

typedef struct {
  unsigned long long packets = 0;
  unsigned long long bytes = 0;
  unsigned long long dropped = 0;
  unsigned long long errors = 0;
} packets_t;

typedef struct {
  string name;
  string address;
  string operstate;
  unsigned long speed;  // bytes/sec
  packets_t tx;
  packets_t rx;
} interface_t;

typedef struct {
  vector<interface_t> interfaces;
  packets_t tx;
  packets_t rx;
} network_t;

void get_packets(interface_t* iface, network_t* net, const char* path) {
  string file_path;

  file_path = path;
  file_path += "/address";
  iface->address = read_file(file_path.c_str());
  iface->operstate = trim(iface->address);

  file_path = path;
  file_path += "/operstate";
  iface->operstate = read_file(file_path.c_str());
  iface->operstate = trim(iface->operstate);

  if (iface->operstate != "up") {
    // don't read speed, otherwise we get a buffer underflow
    iface->speed = 0;
  } else {
    file_path = path;
    file_path += "/speed";
    iface->speed = std::atol(read_file(file_path.c_str()).c_str());
    // the units are stored in MB/s in the file, so multiply to convert into B/s
    iface->speed *= 1e+6;
  }

  char* end_ptr;

  file_path = path;
  file_path += "/statistics/rx_bytes";
  iface->rx.bytes = std::strtoull(read_file(file_path.c_str()).c_str(), &end_ptr, 10);

  file_path = path;
  file_path += "/statistics/rx_packets";
  iface->rx.packets = std::strtoull(read_file(file_path.c_str()).c_str(), &end_ptr, 10);

  file_path = path;
  file_path += "/statistics/rx_dropped";
  iface->rx.dropped = std::strtoull(read_file(file_path.c_str()).c_str(), &end_ptr, 10);

  file_path = path;
  file_path += "/statistics/rx_errors";
  iface->rx.errors = std::strtoull(read_file(file_path.c_str()).c_str(), &end_ptr, 10);

  file_path = path;
  file_path += "/statistics/tx_bytes";
  iface->tx.bytes = std::strtoull(read_file(file_path.c_str()).c_str(), &end_ptr, 10);

  file_path = path;
  file_path += "/statistics/tx_packets";
  iface->tx.packets = std::strtoull(read_file(file_path.c_str()).c_str(), &end_ptr, 10);

  file_path = path;
  file_path += "/statistics/tx_dropped";
  iface->tx.dropped = std::strtoull(read_file(file_path.c_str()).c_str(), &end_ptr, 10);

  file_path = path;
  file_path += "/statistics/tx_errors";
  iface->tx.errors = std::strtoull(read_file(file_path.c_str()).c_str(), &end_ptr, 10);

  net->rx.bytes += iface->rx.bytes;
  net->rx.packets += iface->rx.packets;
  net->rx.dropped += iface->rx.dropped;
  net->rx.errors += iface->rx.errors;

  net->tx.bytes += iface->tx.bytes;
  net->tx.packets += iface->tx.packets;
  net->tx.dropped += iface->tx.dropped;
  net->tx.errors += iface->tx.errors;
}

network_t get_network() {
  network_t result;
  glob_t glob_result;

  glob(INET_GLOB, GLOB_TILDE, NULL, &glob_result);
  for (unsigned i = 0; i < glob_result.gl_pathc; i++) {
    // check if symlink
    struct stat statbuf;

    if (lstat(glob_result.gl_pathv[i], &statbuf) < 0) { /* if error occured */
      std::cerr << "Cannot stat file: " << glob_result.gl_pathv[i] << std::endl;
      globfree(&glob_result);
      exit(EXIT_FAILURE);
    }

    char* real_path;

    if ((real_path = realpath(glob_result.gl_pathv[i], NULL)) != NULL) {
      if (strstr(real_path, "/sys/devices/virtual/") == NULL) {
        // consider this interface
        interface_t iface;

        iface.name = basename(glob_result.gl_pathv[i]);

        get_packets(&iface, &result, real_path);

        result.interfaces.push_back(iface);
      } else {
        // this is a virtual device, ignore
        continue;
      }

    } else {
      std::cerr << "Cannot get path of: " << glob_result.gl_pathv[i] << std::endl;
      globfree(&glob_result);
      free(real_path);
      exit(EXIT_FAILURE);
    }

    free(real_path);
  }

  globfree(&glob_result);
  return result;
}

void pretty_print(network_t info, bool summary = false) {
  fmt_measure_t fmt;

  pretty("Network", "", 0);

  if (summary) {
    fmt = format_storage(info.rx.bytes, 2);
    pretty("Receive", fmt.value.c_str(), 1, NUMBER, 1, fmt.unit.c_str());
    fmt = format_storage(info.tx.bytes, 2);
    pretty("Transmit", fmt.value.c_str(), 1, NUMBER, 0, fmt.unit.c_str());
  } else {
    pretty("Receive", "", 1, STRING);
    fmt = format_storage(info.rx.bytes, 2);
    pretty("Bytes", fmt.value.c_str(), 2, NUMBER, 2, fmt.unit.c_str());
    pretty("Packets", std::to_string(info.rx.packets).c_str(), 2, NUMBER, 0);
    pretty("Dropped", std::to_string(info.rx.dropped).c_str(), 2, NUMBER, 0);
    pretty("Errors", std::to_string(info.rx.errors).c_str(), 2, NUMBER, 1);

    pretty("Transmit", "", 1, STRING);
    fmt = format_storage(info.tx.bytes, 2);
    pretty("Bytes", fmt.value.c_str(), 2, NUMBER, 2, fmt.unit.c_str());
    pretty("Packets", std::to_string(info.tx.packets).c_str(), 2, NUMBER, 0);
    pretty("Dropped", std::to_string(info.tx.dropped).c_str(), 2, NUMBER, 0);
    pretty("Errors", std::to_string(info.tx.errors).c_str(), 2, NUMBER, 1);

    pretty("Interfaces", "", 1);
    for (const auto& iface : info.interfaces) {
      pretty("Name", iface.name.c_str(), 3, STRING, 3, "", true);
      pretty("Address", iface.address.c_str(), 3, STRING);
      pretty("Status", iface.operstate.c_str(), 3, STRING, 1);
      fmt = format_speed(iface.speed, 2);
      pretty("Speed", fmt.value.c_str(), 3, NUMBER, 2, fmt.unit.c_str());
      // pretty("Speed", iface.speed, 3, NUMBER);
      pretty("Receive", "", 3, STRING);
      fmt = format_storage(iface.rx.bytes, 2);
      pretty("Bytes", fmt.value.c_str(), 4, NUMBER, 2, fmt.unit.c_str());
      pretty("Packets", std::to_string(iface.rx.packets).c_str(), 4, NUMBER, 0);
      pretty("Dropped", std::to_string(iface.rx.dropped).c_str(), 4, NUMBER, 0);
      pretty("Errors", std::to_string(iface.rx.errors).c_str(), 4, NUMBER, 1);

      pretty("Transmit", "", 3, STRING);
      fmt = format_storage(iface.tx.bytes, 2);
      pretty("Bytes", fmt.value.c_str(), 4, NUMBER, 2, fmt.unit.c_str());
      pretty("Packets", std::to_string(iface.tx.packets).c_str(), 4, NUMBER, 0);
      pretty("Dropped", std::to_string(iface.tx.dropped).c_str(), 4, NUMBER, 0);
      pretty("Errors", std::to_string(iface.tx.errors).c_str(), 4, NUMBER, 1);
    }
  }
}
