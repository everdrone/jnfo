#pragma once

// #include <ncurses.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "defines.h"

using std::string;
using std::vector;

bool is_sudo_or_root() {
  if (geteuid() == 0) return true;
  return false;
}

enum pretty_type { NORMAL, NUMBER, BOOL, STRING, PERCENT };

void pretty(const char* key, const char* value = "", short level = 0,
            enum pretty_type type = NORMAL, short pad = 0, const char* unit = "",
            bool title = false) {
  string fmt = value;
  string title_mark = "-";
  string unit_str = unit;

  if (unit_str.size() > 0) {
    unit_str = " " + unit_str;
  }

  if (enable_color) {
    if (unit_str.size() > 0) {
      unit_str = ANSI_COLOR_BRIGHT_BLACK + unit_str + ANSI_COLOR_RESET;
    }
    title_mark = ANSI_COLOR_YELLOW + title_mark + ANSI_COLOR_RESET;
    switch (type) {
      case NUMBER:
        fmt = ANSI_COLOR_GREEN + fmt + ANSI_COLOR_RESET;
        break;
      case BOOL:
        fmt = ANSI_COLOR_MAGENTA + fmt + ANSI_COLOR_RESET;
        break;
      case STRING:
        fmt = ANSI_COLOR_CYAN + fmt + ANSI_COLOR_RESET;
        break;
      case PERCENT:
        fmt = ANSI_COLOR_RED + fmt + ANSI_COLOR_RESET;
        break;
      default:
        break;
    }
  }

  if (title) {
    level -= 1;
    printf("%*s%s %s:%*s%s%s\n", level * 2, "", title_mark.c_str(), key, pad + 1, "", fmt.c_str(),
           unit_str.c_str());
  } else {
    printf("%*s%s:%*s%s%s\n", level * 2, "", key, pad + 1, "", fmt.c_str(), unit_str.c_str());
  }
}

void print_indent(const char* str, int level = 0, bool add_newline = false) {
  if (add_newline) {
    printf("%*s%s\n", level, "", str);
  } else {
    printf("%*s%s", level, "", str);
  }
}

string read_file(const char* path) {
  std::ifstream in_stream(path);
  string result;

  result = string((std::istreambuf_iterator<char>(in_stream)), std::istreambuf_iterator<char>());

  return result;
}

// trim from left
inline string& ltrim(string& s, const char* t = " \t\n\r\f\v") {
  s.erase(0, s.find_first_not_of(t));
  return s;
}

// trim from right
inline string& rtrim(string& s, const char* t = " \t\n\r\f\v") {
  s.erase(s.find_last_not_of(t) + 1);
  return s;
}

// trim from left & right
inline string& trim(string& s, const char* t = " \t\n\r\f\v") {
  return ltrim(rtrim(s, t), t);
}

vector<string> split_string(const string& str, const string& delimiter) {
  vector<string> strings;
  string::size_type pos = 0;
  string::size_type prev = 0;
  while ((pos = str.find(delimiter, prev)) != string::npos) {
    strings.push_back(str.substr(prev, pos - prev));
    prev = pos + 1;
  }

  // To get the last substring (or only, if delimiter is not found)
  strings.push_back(str.substr(prev));

  return strings;
}

vector<string> read_lines(const char* path) {
  std::ifstream in_stream(path);
  vector<string> result_lines;

  string result((std::istreambuf_iterator<char>(in_stream)), std::istreambuf_iterator<char>());

  result_lines = split_string(result, "\n");
  return result_lines;
}
