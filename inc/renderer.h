#pragma once
#include <SDL2/SDL_surface.h>

#include <vector>

#include "matrix.hpp"
#include "vertex.h"
#include "color.h"

namespace sf {

class Renderer {
 public:
  Renderer(SDL_Surface* surface);
  void clearColor(const Vector<float, 3>& color);
  void setViewport(int x, int y, int w, int h);
  void setTransform(const Matrix<float, 4>& matrix);
  void fillRect(const Vertex& left_top, const Vertex& right_bottom,
                SDL_Surface* texture = nullptr);
  void drawPoint(const Vertex& point);
  void drawPoints(const std::vector<Vertex>& points);
  void drawLine(const Vertex& p1, const Vertex& p2,
                SDL_Surface* texture = nullptr);
  void drawTriangle(const Vertex& p1, const Vertex& p2, const Vertex& p3,
                    SDL_Surface* texture);
  void fillTriangle(const Vertex& p1, const Vertex& p2, const Vertex& p3,
                    SDL_Surface* texture);
 private:
  Color getTextureColor(SDL_Surface* surface, int x, int y);
  float caculateArea(float a_x, float a_y, float b_x, float b_y, float c_x, float c_y);
  SDL_Surface* surface_;
  Matrix<float, 4> viewport_transform;
  Matrix<float, 4> transform;
};

}  // namespace sf
