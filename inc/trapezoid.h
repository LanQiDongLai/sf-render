#pragma once
#include "point.h"
#include <utility>

namespace sf {

struct Trapezoid {
  float top;
  float bottom;
  std::pair<Point, Point> left_line;
  std::pair<Point, Point> right_line;
  Trapezoid(float top, float bottom, const Point& left_line_start,
            const Point& left_line_end, const Point& right_line_start,
            const Point& right_line_end)
      : top{top},
        bottom{bottom},
        left_line{left_line_start, left_line_end},
        right_line{right_line_start, right_line_end} {}
};

}  // namespace sf
