#pragma once

#include <unistd.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::vector;

bool is_sudo_or_root() {
  if (geteuid() == 0) return true;
  return false;
}

void pretty(const char* key, const char* value = "", int level = 0, bool newline = true,
            bool title = false) {
  if (title) {
    level -= 1;
    if (newline) {
      printf("%*s- %s: %s\n", level * 2, "", key, value);
    } else {
      printf("%*s- %s: %s", level * 2, "", key, value);
    }
  } else {
    if (newline) {
      printf("%*s%s: %s\n", level * 2, "", key, value);
    } else {
      printf("%*s%s: %s", level * 2, "", key, value);
    }
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
