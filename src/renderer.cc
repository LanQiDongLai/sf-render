#include "renderer.h"
#include <algorithm>
#include <cstdio>
#include <cassert>

namespace sf {

Renderer::Renderer(SDL_Surface *surface) { surface_ = surface; }

void Renderer::fillRect(const Point& left_top, const Point& right_bottom, const Color& color) {
  SDL_LockSurface(surface_);
  int surface_w = surface_->w;
  int surface_h = surface_->h;
  Uint32 *pixels = static_cast<Uint32 *>(surface_->pixels);

  if (left_top.x >= surface_w || left_top.y >= surface_h || right_bottom.x < 0 ||
      right_bottom.y < 0) {
    SDL_UnlockSurface(surface_);
    return;
  }
  int right_bound = right_bottom.x >= surface_w ? surface_w - 1 : right_bottom.x;
  int bottom_bound = right_bottom.y >= surface_h ? surface_h - 1 : right_bottom.y;
  int left_bound = left_top.x < 0 ? 0 : left_top.x;
  int top_bound = left_top.y < 0 ? 0 : left_top.y;
  for (int i = left_bound; i <= right_bound; i++) {
    for (int j = top_bound; j <= bottom_bound; j++) {
      pixels[j * surface_w + i] = SDL_MapRGB(surface_->format, color.r, color.g, color.b);
    }
  }
  SDL_UnlockSurface(surface_);
}

void Renderer::drawPoint(const Point &point, const Color &color) {
  SDL_LockSurface(surface_);

  int surface_w = surface_->w;
  int surface_h = surface_->h;
  Uint32 *pixels = static_cast<Uint32 *>(surface_->pixels);

  if (point.x >= surface_w || point.y >= surface_h || point.x < 0 ||
      point.y < 0) {
    SDL_UnlockSurface(surface_);
    return;
  }
  pixels[point.y * surface_w + point.x] =
      SDL_MapRGB(surface_->format, color.r, color.g, color.b);
  SDL_UnlockSurface(surface_);
}

void Renderer::drawPoints(const Point *points, const Color *colors, int count) {
  SDL_LockSurface(surface_);

  int surface_w = surface_->w;
  int surface_h = surface_->h;
  Uint32 *pixels = static_cast<Uint32 *>(surface_->pixels);
  for (int i = 0; i < count; i++) {
    const Point& point = points[i];
    const Color& color = colors[i];
    if (point.x >= surface_w || point.y >= surface_h || point.x < 0 ||
        point.y < 0) {
      continue;
    }
    pixels[point.y * surface_w + point.x] = SDL_MapRGB(
        surface_->format, color.r, color.g, color.b);
  }
  SDL_UnlockSurface(surface_);
}

void Renderer::drawLine(const Point& p1, const Color& c1, const Point& p2, const Color& c2) {
  int x1 = p1.x;
  int y1 = p1.y;
  int x2 = p2.x;
  int y2 = p2.y;

  SDL_LockSurface(surface_);
  int surface_w = surface_->w;
  int surface_h = surface_->h;
  Uint32 *pixels = static_cast<Uint32 *>(surface_->pixels);

  bool steep = std::abs(y2 - y1) > std::abs(x2 - x1);
  if(steep) {
    std::swap(x1, y1);
    std::swap(x2, y2);
  }
  if(x1 > x2) {
    std::swap(x1, x2);
    std::swap(y1, y2);
  }
  int deltax = x2 - x1;
  int deltay = std::abs(y2 - y1);
  int error = deltax / 2;
  int ystep;
  int y = y1; 
  if(y1 < y2) {
    ystep = 1;
  }
  else {
    ystep = -1;
  }
  float t = 0;
  for(int x = x1; x <= x2; x++) {
    t = (float)(x - x1) / (float)(x2 - x1);
    Uint32 mixed_color = SDL_MapRGB(surface_->format, (1 - t) * c1.r + t * c2.r,
                       (1 - t) * c1.g + t * c2.g,
                       (1 - t) * c1.b + t * c2.b);
    if(steep) {
      int pos = x * surface_w + y;
      if(pos >= 0 && pos < surface_w * surface_h) {
        pixels[pos] = mixed_color;
      }
    }
    else {
      int pos = y * surface_w + x;
      if(pos >= 0 && pos < surface_w * surface_h)
        pixels[pos] = mixed_color;
    }
    error -= deltay;
    if (error < 0) {
      y = y + ystep;
      error += deltax;
    }
  }
  SDL_UnlockSurface(surface_);
}

} // namespace sf
