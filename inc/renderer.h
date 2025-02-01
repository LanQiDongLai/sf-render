#pragma once
#include <SDL2/SDL_surface.h>

#include <vector>

#include "point.hpp"

namespace sf {

class Renderer {
 public:
  Renderer(SDL_Surface* surface);
  void fillRect(const Point<int>& left_top, const Point<int>& right_bottom,
                const Color& color);
  void drawPoint(const Point<int>& point, const Color& color);
  void drawPoints(const Point<int>* points, const Color* colors, int count);
  void drawLine(const Point<int>& p1, const Color& c1, const Point<int>& p2,
                const Color& c2);
  void drawLine(const Point<int>& p1, const Point<float>& uv1,
                const Point<int>& p2, const Point<float>& uv2,
                SDL_Surface* texture);
  void drawTriangle(const Point<int>& p1, const Color& c1, const Point<int>& p2,
                    const Color& c2, const Point<int>& p3, const Color& c3);
  void drawTriangle(const Point<int>& p1, const Point<float>& uv1, const Point<int>& p2,
                    const Point<float>& uv2, const Point<int>& p3, const Point<float>& uv3);
  void drawTrapezoid(int top, int top_left, const Color& left_top_color,
                     int top_right, const Color& right_top_color,
                     int bottom, int bottom_left,
                     const Color& left_bottom_color, int bottom_right,
                     const Color& right_bottom_color);
  void drawTrapezoid(int top, int top_left, const Point<float>& left_top_uv,
                     int top_right, const Point<float>& right_top_uv,
                     int bottom, int bottom_left,
                     const Point<float>& left_bottom_uv, int bottom_right,
                     const Point<float>& right_bottom_uv, SDL_Surface* texture);

 private:
  Color getTextureColor(SDL_Surface* surface, int x, int y);
  SDL_Surface* surface_;
};

}  // namespace sf
