#pragma once
#include "point.hpp"
#include <SDL2/SDL_surface.h>
#include <vector>

namespace sf {

class Renderer {
public:
  Renderer(SDL_Surface *surface);
  void fillRect(const Point<int> &left_top, const Point<int> &right_bottom, const Color& color);
  void drawPoint(const Point<int> &point, const Color& color);
  void drawPoints(const Point<int> *points, const Color *colors, int count);
  void drawLine(const Point<int>& p1, const Color& c1, const Point<int>& p2, const Color& c2);
  void drawLine(const Point<int>& p1, const Point<float>& uv1, const Point<int>& p2, const Point<float>& uv2, SDL_Surface* texture);
private:
  Color getTextureColor(SDL_Surface *surface, int x, int y);
  SDL_Surface *surface_;
};

} // namespace sf
