#pragma once
#include "color.h"

namespace sf {
template <class T>
struct Point {
  T x;
  T y;
  Point(T x, T y) : x{x}, y{y} {};
};

}  // namespace sf
