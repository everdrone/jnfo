#pragma once

#include <string.h>
#include <sys/statvfs.h>

#include "defines.h"
#include "formatter.h"

typedef struct {
  string name;
  string path;
  unsigned long capacity;
  unsigned long free;
  unsigned long available;
  unsigned long used;
  unsigned long nonroot_total;
} fs_t;

vector<fs_t> get_filesystem() {
  vector<fs_t> result;

  FILE* fd = fopen(MTAB_PATH, "r");

  if (fd == NULL) {
    std::cerr << "cannot open " << MTAB_PATH << std::endl;
    exit(EXIT_FAILURE);
  }

  char line[128];
  char part_name[32];
  char mount_path[32];

  while (fgets(line, 128, fd) != NULL) {
    // parse line
    if (sscanf(line, "%s %s", part_name, mount_path) == 2) {
      int num_slashes = 0;
      char* ptr = mount_path;

      // count forward slashes in path
      while ((ptr = strchr(ptr, '/')) != NULL) {
        num_slashes++, ptr++;
      }

      if (num_slashes == 1) {
        // consider filesystem
        fs_t filesystem;
        filesystem.path = mount_path;
#if __APPLE__
        if (filesystem.path == "/") {
#endif
          filesystem.name = part_name;

          struct statvfs vfs;

          int err = statvfs(filesystem.path.c_str(), &vfs);
          if (err != 0) {
            std::cerr << "Cannot stat filesystem." << std::endl;
            std::cerr << err << std::endl;
            fclose(fd);
            exit(EXIT_FAILURE);
          }

          filesystem.capacity = vfs.f_blocks * vfs.f_frsize;
          filesystem.free = vfs.f_bfree * vfs.f_frsize;
          filesystem.available = vfs.f_bavail * vfs.f_frsize;

          filesystem.used = filesystem.capacity - filesystem.free;
          filesystem.nonroot_total = filesystem.used + filesystem.available;

          result.push_back(filesystem);
        }

#if __APPLE__
      }
#endif
    } else {
      std::cerr << "cannot parse " << MTAB_PATH << std::endl;
      fclose(fd);
      exit(EXIT_FAILURE);
    }
  }

  fclose(fd);

  return result;
}

void pretty_print(vector<fs_t> info, bool summary = false) {
  fmt_measure_t fmt;

  pretty("Filesystem", "", 0);

  for (const auto& fs : info) {
    if (summary) {
      if (fs.path == "/") {
        fmt = format_storage(fs.capacity, 2);
        pretty("Capacity", fmt.value.c_str(), 1, NUMBER, 0, fmt.unit.c_str());

        pretty("Used", format_percent(fs.used, fs.nonroot_total, 0).c_str(), 1, PERCENT, 4);
        break;
      }
    } else {
      pretty("Name", fs.name.c_str(), 2, STRING, 5, "", true);
      pretty("Path", fs.path.c_str(), 2, STRING, 5);

      fmt = format_storage(fs.capacity, 2);
      pretty("Capacity", fmt.value.c_str(), 2, NUMBER, 1, fmt.unit.c_str());

      fmt = format_storage(fs.free, 2);
      pretty("Free", fmt.value.c_str(), 2, NUMBER, 5, fmt.unit.c_str());

      fmt = format_storage(fs.available, 2);
      pretty("Available", fmt.value.c_str(), 2, NUMBER, 0, fmt.unit.c_str());

      pretty("Used", format_percent(fs.used, fs.nonroot_total, 0).c_str(), 2, PERCENT, 5);
    }
  }
}
