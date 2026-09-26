#pragma once

#include <algorithm>
#include <cstddef>

class Print {
 public:
  std::size_t print(char) { return 0; }

  template <typename T>
  std::size_t print(const T&) {
    return 0;
  }
};

using std::max;
