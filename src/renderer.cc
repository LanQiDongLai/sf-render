#include "renderer.h"

#include <SDL2/SDL_surface.h>

#include <algorithm>
#include <cassert>
#include <cstdio>

namespace sf {

Renderer::Renderer(SDL_Surface* surface) { surface_ = surface; }

void Renderer::setViewport(int x, int y, int w, int h) {
  viewport_transform[0][0] = w / 2.;
  viewport_transform[1][1] = -h / 2.;
  viewport_transform[0][3] = (float)x + w / 2.;
  viewport_transform[1][3] = (float)y + h / 2.;
}

void Renderer::setTransform(const Matrix<float, 4>& matrix) {
  transform = matrix;
}

void Renderer::fillRect(const Vertex& left_top, const Vertex& right_bottom,
                        SDL_Surface* texture) {}

void Renderer::drawPoint(const Vertex& point) {
  sf::Vector<float, 4> pos_on_screen =
      viewport_transform * transform * point.position;
  if (pos_on_screen[0] >= surface_->w || pos_on_screen[0] < 0 ||
      pos_on_screen[1] >= surface_->h || pos_on_screen[1] < 0) {
    return;
  }
  Uint32* pixels = static_cast<Uint32*>(surface_->pixels);
  pixels[(int)pos_on_screen[0] + (int)pos_on_screen[1] * surface_->w] =
      SDL_MapRGB(surface_->format, 255 * point.color[0], 255 * point.color[1],
                 255 * point.color[2]);
}

void Renderer::drawPoints(const std::vector<Vertex>& points) {
  Uint32* pixels = static_cast<Uint32*>(surface_->pixels);
  for (const auto& point : points) {
    sf::Vector<float, 4> pos_on_screen =
        viewport_transform * transform * point.position;
    if (pos_on_screen[0] >= surface_->w || pos_on_screen[0] < 0 ||
        pos_on_screen[1] >= surface_->h || pos_on_screen[1] < 0) {
      return;
    }
    pixels[(int)pos_on_screen[0] + (int)pos_on_screen[1] * surface_->w] =
        SDL_MapRGB(surface_->format, 255 * point.color[0], 255 * point.color[1],
                   255 * point.color[2]);
  }
}

void Renderer::drawLine(const Vertex& p1, const Vertex& p2,
                        SDL_Surface* texture) {
  auto pos1 = viewport_transform * transform * p1.position;
  auto pos2 = viewport_transform * transform * p2.position;
  auto uv1 = p1.texcoord;
  int x1 = pos1[0] / pos1[3];
  int y1 = pos1[1] / pos1[3];
  int x2 = pos2[0] / pos2[3];
  int y2 = pos2[1] / pos2[3];

  float rz1 = p1.position[2];
  float rz2 = p2.position[2];

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
    std::swap(rz1, rz2);
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
    float rz = rz1 * rz2 / ((1 - t) * rz2 + t * rz1);
    Uint32 mixed_color = SDL_MapRGB(
        surface_->format,
        255 * rz * ((1 - t) * p1.color[0] / rz1 + t * p2.color[0] / rz2),
        255 * rz * ((1 - t) * p1.color[1] / rz1 + t * p2.color[1] / rz2),
        255 * rz * ((1 - t) * p1.color[2] / rz1 + t * p1.color[2] / rz2));
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
}

void Renderer::drawTriangle(const Vertex& p1, const Vertex& p2,
                            const Vertex& p3, SDL_Surface* texture) {}

void Renderer::drawTrapezoid(const Vertex& left_top, const Vertex& right_top,
                             const Vertex& left_bottom,
                             const Vertex& right_bottom, SDL_Surface* texture) {
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

}  // namespace sf
