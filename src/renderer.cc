#include "renderer.h"
#include <algorithm>
#include <cstdio>

namespace sf {

Renderer::Renderer(SDL_Surface *surface) { surface_ = surface; }

void Renderer::fillRect(int x, int y, int w, int h, unsigned int color) {
  SDL_LockSurface(surface_);
  int surface_w = surface_->w;
  int surface_h = surface_->h;
  Uint32 *pixels = static_cast<Uint32 *>(surface_->pixels);
  int r = (color & 0xff000000) >> 24;
  int g = (color & 0x00ff0000) >> 16;
  int b = (color & 0x0000ff00) >> 8;
  int a = (color & 0x000000ff) >> 0;
  if (x >= surface_w || y >= surface_h || x + surface_w < 0 ||
      x + surface_h < 0) {
    SDL_UnlockSurface(surface_);
    return;
  }
  int right_bound = (x + w) >= surface_w ? surface_w - 1 : x + w;
  int bottom_bound = (y + h) >= surface_h ? surface_h - 1 : y + h;
  int left_bound = x < 0 ? 0 : x;
  int top_bound = y < 0 ? 0 : y;
  for (int i = left_bound; i <= right_bound; i++) {
    for (int j = top_bound; j <= bottom_bound; j++) {
      pixels[j * surface_w + i] = SDL_MapRGB(surface_->format, r, g, b);
    }
  }
  SDL_UnlockSurface(surface_);
}

void Renderer::drawPoint(const Point &point) {
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
      SDL_MapRGB(surface_->format, point.color.r, point.color.g, point.color.b);
  SDL_UnlockSurface(surface_);
}

void Renderer::drawPoints(const std::vector<Point>& points) {
  SDL_LockSurface(surface_);

  int surface_w = surface_->w;
  int surface_h = surface_->h;
  Uint32 *pixels = static_cast<Uint32 *>(surface_->pixels);
  for (const auto &point : points) {
    if (point.x >= surface_w || point.y >= surface_h || point.x < 0 ||
        point.y < 0) {
      continue;
    }
    pixels[point.y * surface_w + point.x] = SDL_MapRGB(
        surface_->format, point.color.r, point.color.g, point.color.b);
  }
  SDL_UnlockSurface(surface_);
}

void Renderer::drawLine(const Point& p1, const Point& p2) {
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
    Uint32 mixed_color = SDL_MapRGB(surface_->format, (1 - t) * p1.color.r + t * p2.color.r,
                       (1 - t) * p1.color.g + t * p2.color.g,
                       (1 - t) * p1.color.b + t * p2.color.b);
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
