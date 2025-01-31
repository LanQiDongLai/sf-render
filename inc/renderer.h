#pragma once
#include "point.h"
#include <SDL2/SDL_surface.h>
#include <vector>

namespace sf {

class Renderer {
public:
  Renderer(SDL_Surface *surface);
  void fillRect(int x, int y, int w, int h, unsigned int color);
  void drawPoint(const Point &point);
  void drawPoints(const std::vector<Point>& points);
  void drawLine(const Point& p1, const Point& p2);
private:
  SDL_Surface *surface_;
};

} // namespace sf
