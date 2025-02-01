#include "renderer.h"

#include <SDL2/SDL_surface.h>

#include <algorithm>
#include <cassert>
#include <cstdio>

namespace sf {

Renderer::Renderer(SDL_Surface* surface) { surface_ = surface; }

void Renderer::fillRect(const Point<int>& left_top,
                        const Point<int>& right_bottom, const Color& color) {
  SDL_LockSurface(surface_);
  int surface_w = surface_->w;
  int surface_h = surface_->h;
  Uint32* pixels = static_cast<Uint32*>(surface_->pixels);

  if (left_top.x >= surface_w || left_top.y >= surface_h ||
      right_bottom.x < 0 || right_bottom.y < 0) {
    SDL_UnlockSurface(surface_);
    return;
  }
  int right_bound =
      right_bottom.x >= surface_w ? surface_w - 1 : right_bottom.x;
  int bottom_bound =
      right_bottom.y >= surface_h ? surface_h - 1 : right_bottom.y;
  int left_bound = left_top.x < 0 ? 0 : left_top.x;
  int top_bound = left_top.y < 0 ? 0 : left_top.y;
  for (int i = left_bound; i <= right_bound; i++) {
    for (int j = top_bound; j <= bottom_bound; j++) {
      pixels[j * surface_w + i] =
          SDL_MapRGB(surface_->format, color.r, color.g, color.b);
    }
  }
  SDL_UnlockSurface(surface_);
}

void Renderer::drawPoint(const Point<int>& point, const Color& color) {
  SDL_LockSurface(surface_);

  int surface_w = surface_->w;
  int surface_h = surface_->h;
  Uint32* pixels = static_cast<Uint32*>(surface_->pixels);

  if (point.x >= surface_w || point.y >= surface_h || point.x < 0 ||
      point.y < 0) {
    SDL_UnlockSurface(surface_);
    return;
  }
  pixels[point.y * surface_w + point.x] =
      SDL_MapRGB(surface_->format, color.r, color.g, color.b);
  SDL_UnlockSurface(surface_);
}

void Renderer::drawPoints(const Point<int>* points, const Color* colors,
                          int count) {
  SDL_LockSurface(surface_);

  int surface_w = surface_->w;
  int surface_h = surface_->h;
  Uint32* pixels = static_cast<Uint32*>(surface_->pixels);
  for (int i = 0; i < count; i++) {
    const Point<int>& point = points[i];
    const Color& color = colors[i];
    if (point.x >= surface_w || point.y >= surface_h || point.x < 0 ||
        point.y < 0) {
      continue;
    }
    pixels[point.y * surface_w + point.x] =
        SDL_MapRGB(surface_->format, color.r, color.g, color.b);
  }
  SDL_UnlockSurface(surface_);
}

void Renderer::drawLine(const Point<int>& p1, const Color& c1,
                        const Point<int>& p2, const Color& c2) {
  int x1 = p1.x;
  int y1 = p1.y;
  int x2 = p2.x;
  int y2 = p2.y;
  Color tc1 = c1;
  Color tc2 = c2;

  SDL_LockSurface(surface_);
  int surface_w = surface_->w;
  int surface_h = surface_->h;
  Uint32* pixels = static_cast<Uint32*>(surface_->pixels);

  bool steep = std::abs(y2 - y1) > std::abs(x2 - x1);
  if (steep) {
    std::swap(x1, y1);
    std::swap(x2, y2);
  }
  if (x1 > x2) {
    std::swap(x1, x2);
    std::swap(y1, y2);
    std::swap(tc1, tc2);
  }
  int deltax = x2 - x1;
  int deltay = std::abs(y2 - y1);
  int error = deltax / 2;
  int ystep;
  int y = y1;
  if (y1 < y2) {
    ystep = 1;
  } else {
    ystep = -1;
  }
  float t = 0;
  for (int x = x1; x <= x2; x++) {
    t = (float)(x - x1) / (float)(x2 - x1);
    Uint32 mixed_color =
        SDL_MapRGB(surface_->format, (1 - t) * tc1.r + t * tc2.r,
                   (1 - t) * tc1.g + t * tc2.g, (1 - t) * tc1.b + t * tc2.b);
    if (steep) {
      int pos = x * surface_w + y;
      if (pos >= 0 && pos < surface_w * surface_h) {
        pixels[pos] = mixed_color;
      }
    } else {
      int pos = y * surface_w + x;
      if (pos >= 0 && pos < surface_w * surface_h) pixels[pos] = mixed_color;
    }
    error -= deltay;
    if (error < 0) {
      y = y + ystep;
      error += deltax;
    }
  }
  SDL_UnlockSurface(surface_);
}

void Renderer::drawLine(const Point<int>& p1, const Point<float>& uv1,
                        const Point<int>& p2, const Point<float>& uv2,
                        SDL_Surface* texture) {
  int x1 = p1.x;
  int y1 = p1.y;
  int x2 = p2.x;
  int y2 = p2.y;

  Point<float> tuv1 = uv1;
  Point<float> tuv2 = uv2;

  SDL_LockSurface(surface_);
  int surface_w = surface_->w;
  int surface_h = surface_->h;
  Uint32* pixels = static_cast<Uint32*>(surface_->pixels);

  bool steep = std::abs(y2 - y1) > std::abs(x2 - x1);
  if (steep) {
    std::swap(x1, y1);
    std::swap(x2, y2);
  }
  if (x1 > x2) {
    std::swap(x1, x2);
    std::swap(y1, y2);
    std::swap(tuv1, tuv2);
  }
  int deltax = x2 - x1;
  int deltay = std::abs(y2 - y1);
  int error = deltax / 2;
  int ystep;
  int y = y1;
  if (y1 < y2) {
    ystep = 1;
  } else {
    ystep = -1;
  }
  float t = 0;
  for (int x = x1; x <= x2; x++) {
    t = (float)(x - x1) / (float)(x2 - x1);
    float u = (1 - t) * tuv1.x + t * tuv2.x;
    float v = (1 - t) * tuv1.y + t * tuv2.y;
    Color texture_color =
        getTextureColor(texture, u * texture->w, (1 - v) * texture->h);
    Uint32 color = SDL_MapRGB(surface_->format, texture_color.r,
                              texture_color.g, texture_color.b);
    if (steep) {
      int pos = x * surface_w + y;
      if (pos >= 0 && pos < surface_w * surface_h) {
        pixels[pos] = color;
      }
    } else {
      int pos = y * surface_w + x;
      if (pos >= 0 && pos < surface_w * surface_h) {
        pixels[pos] = color;
      }
    }
    error -= deltay;
    if (error < 0) {
      y = y + ystep;
      error += deltax;
    }
  }
  SDL_UnlockSurface(surface_);
}

Color Renderer::getTextureColor(SDL_Surface* surface, int x, int y) {
  Color color = {0, 0, 0, 0};
  if (!surface || x < 0 || x >= surface->w || y < 0 || y >= surface->h) {
    return color;
  }

  int bpp = surface->format->BytesPerPixel;
  Uint8* pixel_ptr = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

  Uint32 pixel_value;
  switch (bpp) {
    case 1:
      pixel_value = *pixel_ptr;
      break;
    case 2:
      pixel_value = *(Uint16*)pixel_ptr;
      break;
    case 3:
      if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
        pixel_value = pixel_ptr[0] << 16 | pixel_ptr[1] << 8 | pixel_ptr[2];
      } else {
        pixel_value = pixel_ptr[0] | pixel_ptr[1] << 8 | pixel_ptr[2] << 16;
      }
      break;
    case 4:
      pixel_value = *(Uint32*)pixel_ptr;
      break;
    default:
      pixel_value = 0;
      break;
  }

  if (surface->format->palette) {
    SDL_Color* palette = surface->format->palette->colors;
    color.r = palette[pixel_value].r;
    color.g = palette[pixel_value].g;
    color.b = palette[pixel_value].b;
    color.a = palette[pixel_value].a;
  } else {
    SDL_GetRGBA(pixel_value, surface->format, &color.r, &color.g, &color.b,
                &color.a);
  }

  return color;
}

void Renderer::drawTriangle(const Point<int>& p1, const Color& c1,
                            const Point<int>& p2, const Color& c2,
                            const Point<int>& p3, const Color& c3) {
  Point<int> tp1 = p1;
  Point<int> tp2 = p2;
  Point<int> tp3 = p3;

  Color tc1 = c1;
  Color tc2 = c2;
  Color tc3 = c3;

  if(tp1.y > tp2.y) {
    std::swap(tp1, tp2);
    std::swap(tc1, tc2);
  }
  if(tp2.y > tp3.y) {
    std::swap(tp2, tp3);
    std::swap(tc2, tc3);
  }
  if(tp1.y > tp2.y) {
    std::swap(tp1, tp2);
    std::swap(tc1, tc2);
  }

  if(tp1.y == tp2.y) {
    drawTrapezoid(tp1.y, tp1.x, tc1, tp2.x, tc2, tp3.y, tp3.x, tc3, tp3.x, tc3);
    return;
  }
  if(tp2.y == tp3.y) {
    drawTrapezoid(tp1.y, tp1.x, tc1, tp1.x, tc1, tp3.y, tp2.x, tc2, tp3.x, tc3);
    return;
  }
  float t = (float)(tp2.y - tp1.y) / (float)(tp3.y - tp1.y);
  int mid_x = (1 - t) * tp1.x + t * tp3.x;
  Color mid_color{
    .r = (unsigned char)((1 - t) * tc1.r + t * tc3.r),
    .g = (unsigned char)((1 - t) * tc1.g + t * tc3.g),
    .b = (unsigned char)((1 - t) * tc1.b + t * tc3.b) 
  };
  drawTrapezoid(tp1.y, tp1.x, tc1, tp1.x, tc1, tp2.y, mid_x, mid_color, tp2.x, tc2);
  drawTrapezoid(tp2.y, mid_x, mid_color, tp2.x, tc2, tp3.y, tp3.x, tc3, tp3.x, tc3);
}

void Renderer::drawTriangle(const Point<int>& p1, const Point<float>& uv1,
                            const Point<int>& p2, const Point<float>& uv2,
                            const Point<int>& p3, const Point<float>& uv3,
                            SDL_Surface* texture) {
  Point<int> tp1 = p1;
  Point<int> tp2 = p2;
  Point<int> tp3 = p3;

  Point<float> tuv1 = uv1;
  Point<float> tuv2 = uv2;
  Point<float> tuv3 = uv3;

  if(tp1.y > tp2.y) {
    std::swap(tp1, tp2);
    std::swap(tuv1, tuv2);
  }
  if(tp2.y > tp3.y) {
    std::swap(tp2, tp3);
    std::swap(tuv2, tuv3);
  }
  if(tp1.y > tp2.y) {
    std::swap(tp1, tp2);
    std::swap(tuv1, tuv2);
  }

  if(tp1.y == tp2.y) {
    drawTrapezoid(tp1.y, tp1.x, tuv1, tp2.x, tuv2, tp3.y, tp3.x, tuv3, tp3.x, tuv3, texture);
    return;
  }
  if(tp2.y == tp3.y) {
    drawTrapezoid(tp1.y, tp1.x, tuv1, tp1.x, tuv1, tp3.y, tp2.x, tuv2, tp3.x, tuv3, texture);
    return;
  }
  float t = (float)(tp2.y - tp1.y) / (float)(tp3.y - tp1.y);
  int mid_x = (1 - t) * tp1.x + t * tp3.x;
  Point<float> mid_uv((1 - t) * tuv1.x + t * tuv3.x, (1 - t) * tuv1.y + t * tuv3.y);
  drawTrapezoid(tp1.y, tp1.x, tuv1, tp1.x, tuv1, tp2.y, mid_x, mid_uv, tp2.x, tuv2, texture);
  drawTrapezoid(tp2.y, mid_x, mid_uv, tp2.x, tuv2, tp3.y, tp3.x, tuv3, tp3.x, tuv3, texture);
}

void Renderer::drawTrapezoid(int top, int top_left,
                             const Color& left_top_color, int top_right,
                             const Color& right_top_color, int bottom,
                             int bottom_left, const Color& left_bottom_color,
                             int bottom_right,
                             const Color& right_bottom_color) {
  for(int cy = top; cy <= bottom; cy++) {
    float t = (float)(cy - top) / (float)(bottom - top);
    Color left_mixed_color{
        .r = (unsigned char)((1 - t) * left_top_color.r + t * left_bottom_color.r),
        .g = (unsigned char)((1 - t) * left_top_color.g + t * left_bottom_color.g),
        .b = (unsigned char)((1 - t) * left_top_color.b + t * left_bottom_color.b)};
    Color right_mixed_color{
        .r = (unsigned char)((1 - t) * right_top_color.r + t * right_bottom_color.r),
        .g = (unsigned char)((1 - t) * right_top_color.g + t * right_bottom_color.g),
        .b = (unsigned char)((1 - t) * right_top_color.b + t * right_bottom_color.b)};
    int left_x = (top_left * bottom + bottom_left * cy - top_left * cy -
                  bottom_left * top) /
                 (bottom - top);
    int right_x = (top_right * bottom + bottom_right * cy - top_right * cy -
                   bottom_right * top) /
                  (bottom - top);
    drawLine(Point<int>{left_x, cy}, left_mixed_color, Point<int>{right_x, cy}, right_mixed_color);
  }
}

void Renderer::drawTrapezoid(
    int top, int top_left, const Point<float>& left_top_uv, int top_right,
    const Point<float>& right_top_uv, int bottom, int bottom_left,
    const Point<float>& left_bottom_uv, int bottom_right,
    const Point<float>& right_bottom_uv, SDL_Surface* texture) {
  for(int cy = top; cy <= bottom; cy++) {
    float t = (float)(cy - top) / (float)(bottom - top);
    Point<float> left_mixed_uv((1 - t) * left_top_uv.x + t * left_bottom_uv.x,
                               (1 - t) * left_top_uv.y + t * left_bottom_uv.y);
    Point<float> right_mixed_uv((1 - t) * right_top_uv.x + t * right_bottom_uv.x,
                               (1 - t) * right_top_uv.y + t * right_bottom_uv.y);
    int left_x = (top_left * bottom + bottom_left * cy - top_left * cy -
                  bottom_left * top) /
                 (bottom - top);
    int right_x = (top_right * bottom + bottom_right * cy - top_right * cy -
                   bottom_right * top) /
                  (bottom - top);
    drawLine(Point<int>{left_x, cy}, left_mixed_uv, Point<int>{right_x, cy}, right_mixed_uv, texture);
  }
}

}  // namespace sf
