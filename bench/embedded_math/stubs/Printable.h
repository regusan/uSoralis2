#pragma once

#include <cstddef>

class Print;

class Printable {
 public:
  virtual ~Printable() = default;
  virtual std::size_t printTo(Print& p) const = 0;
};
