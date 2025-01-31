#pragma once
#include "color.h"

namespace sf {

struct Point {
  int x;
  int y;
  sf::Color color;
  Point(int x, int y, sf::Color color);
};

} // namespace sf
