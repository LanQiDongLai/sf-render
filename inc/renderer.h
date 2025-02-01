#pragma once
#include "point.h"
#include <SDL2/SDL_surface.h>
#include <vector>

namespace sf {

class Renderer {
public:
  Renderer(SDL_Surface *surface);
  void fillRect(const Point &left_top, const Point &right_bottom, const Color& color);
  void drawPoint(const Point &point, const Color& color);
  void drawPoints(const Point *points, const Color *colors, int count);
  void drawLine(const Point& p1, const Color& c1, const Point& p2, const Color& c2);
private:
  SDL_Surface *surface_;
};

} // namespace sf
