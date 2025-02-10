#include "renderer.h"

#include <SDL2/SDL_surface.h>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>

namespace sf {

Renderer::Renderer(SDL_Surface* surface) { surface_ = surface; }

void Renderer::clearColor(const Vector<float, 3>& color) {
  Uint32 *pixels = static_cast<Uint32 *>(surface_->pixels);
  for(int i = 0; i < surface_->h; i++) {
    for(int j = 0; j < surface_->w; j++) {
      pixels[i * surface_->w + j] = SDL_MapRGB(surface_->format, 255 * color[0],
                                               255 * color[1], 255 * color[2]);
    }
  }
}

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
  auto sp1 = p1;
  auto sp2 = p2;
  auto transformed_p1 = transform * p1.position;
  auto transformed_p2 = transform * p2.position;

  auto screen_p1 = viewport_transform * transformed_p1;
  screen_p1[0] /= screen_p1[3];
  screen_p1[1] /= screen_p1[3];
  screen_p1[2] /= screen_p1[3];
  screen_p1[3] /= screen_p1[3];
  auto screen_p2 = viewport_transform * transformed_p2;
  screen_p2[0] /= screen_p2[3];
  screen_p2[1] /= screen_p2[3];
  screen_p2[2] /= screen_p2[3];
  screen_p2[3] /= screen_p2[3];

  int surface_w = surface_->w;
  int surface_h = surface_->h;
  Uint32* pixels = static_cast<Uint32*>(surface_->pixels);

  bool steep = std::abs(screen_p2[1] - screen_p1[1]) >
               std::abs(screen_p2[0] - screen_p1[0]);
  if (steep) {
    std::swap(screen_p1[0], screen_p1[1]);
    std::swap(screen_p2[0], screen_p2[1]);
  }
  if (screen_p1[0] > screen_p2[0]) {
    std::swap(screen_p1, screen_p2);
    std::swap(transformed_p1, transformed_p2);
    std::swap(sp1, sp2);
  }

  int x1 = screen_p1[0];
  int y1 = screen_p1[1];
  int x2 = screen_p2[0];
  int y2 = screen_p2[1];

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
  float rz1 = transformed_p1[3];
  float rz2 = transformed_p2[3];
  for (int x = x1; x <= x2; x++) {
    t = (float)(x - x1) / (float)(x2 - x1);
    float rz = rz1 * rz2 / ((1 - t) * rz2 + t * rz1);
    Uint32 mixed_color;
    if(texture == nullptr) {
      mixed_color = SDL_MapRGB(
          surface_->format,
          255 * rz * ((1 - t) * sp1.color[0] / rz1 + t * sp2.color[0] / rz2),
          255 * rz * ((1 - t) * sp1.color[1] / rz1 + t * sp2.color[1] / rz2),
          255 * rz * ((1 - t) * sp1.color[2] / rz1 + t * sp2.color[2] / rz2));
    } else {
      float u = rz * ((1 - t) * sp1.texcoord[0] / rz1 + t * sp2.texcoord[0] / rz2);
      float v = rz * ((1 - t) * sp1.texcoord[1] / rz1 + t * sp2.texcoord[1] / rz2);
      auto color = getTextureColor(texture, u * texture->w, v * texture->h);
      mixed_color = SDL_MapRGB(surface_->format, color.r, color.g, color.b);
    }
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
                            const Vertex& p3, SDL_Surface* texture) {
  drawLine(p1, p2, texture);
  drawLine(p2, p3, texture);
  drawLine(p3, p1, texture);
}

void Renderer::fillTriangle(const Vertex& p1, const Vertex& p2,
                            const Vertex& p3, SDL_Surface* texture) {
  auto sp1 = p1;
  auto sp2 = p2;
  auto sp3 = p3;

  Uint32* pixels = static_cast<Uint32*>(surface_->pixels);

  auto transformed_p1 = transform * sp1.position;
  auto transformed_p2 = transform * sp2.position;
  auto transformed_p3 = transform * sp3.position;

  float rz1 = transformed_p1[3];
  float rz2 = transformed_p2[3];
  float rz3 = transformed_p3[3];

  auto screen_p1 = viewport_transform * transformed_p1;
  screen_p1[0] /= screen_p1[3];
  screen_p1[1] /= screen_p1[3];
  screen_p1[2] /= screen_p1[3];
  screen_p1[3] /= screen_p1[3];
  auto screen_p2 = viewport_transform * transformed_p2;
  screen_p2[0] /= screen_p2[3];
  screen_p2[1] /= screen_p2[3];
  screen_p2[2] /= screen_p2[3];
  screen_p2[3] /= screen_p2[3];
  auto screen_p3 = viewport_transform * transformed_p3;
  screen_p3[0] /= screen_p3[3];
  screen_p3[1] /= screen_p3[3];
  screen_p3[2] /= screen_p3[3];
  screen_p3[3] /= screen_p3[3];

  float top = std::min(std::min(screen_p1[1], screen_p2[1]), screen_p3[1]);
  float bottom = std::max(std::max(screen_p1[1], screen_p2[1]), screen_p3[1]);
  float left = std::min(std::min(screen_p1[0], screen_p2[0]), screen_p3[0]);
  float right = std::max(std::max(screen_p1[0], screen_p2[0]), screen_p3[0]);

  float area = caculateArea(screen_p1[0], screen_p1[1], screen_p2[0],
                            screen_p2[1], screen_p3[0], screen_p3[1]);
  if (area < 0.00001f) return;

  for (int cy = top; cy <= bottom; cy++) {
    for (int cx = left; cx <= right; cx++) {
      if (cx < 0 || cy < 0 || cx >= surface_->w || cy >= surface_->h) continue;
      bool b1 = (cx - screen_p1[0]) * (screen_p2[1] - screen_p1[1]) -
                    (cy - screen_p1[1]) * (screen_p2[0] - screen_p1[0]) >
                0;
      bool b2 = (cx - screen_p2[0]) * (screen_p3[1] - screen_p2[1]) -
                    (cy - screen_p2[1]) * (screen_p3[0] - screen_p2[0]) >
                0;
      bool b3 = (cx - screen_p3[0]) * (screen_p1[1] - screen_p3[1]) -
                    (cy - screen_p3[1]) * (screen_p1[0] - screen_p3[0]) >
                0;
      if (!(b1 && b2 && b3)) continue;
      float alpha = caculateArea(cx, cy, screen_p2[0], screen_p2[1],
                                 screen_p3[0], screen_p3[1]) /
                    area;
      float beta = caculateArea(screen_p1[0], screen_p1[1], cx, cy,
                                screen_p3[0], screen_p3[1]) /
                   area;
      float gamma = caculateArea(screen_p1[0], screen_p1[1], screen_p2[0],
                                 screen_p2[1], cx, cy) /
                    area;
      float rz = 1. / (alpha / rz1 + beta / rz2 + gamma / rz3);
      Uint32 mixed_color;
      if(texture == nullptr) {
        mixed_color = SDL_MapRGB(
          surface_->format,
          255 * rz *
              ((alpha / rz1) * sp1.color[0] + (beta / rz2) * sp2.color[0] +
               (gamma / rz3) * sp3.color[0]),
          255 * rz *
              ((alpha / rz1) * sp1.color[1] + (beta / rz2) * sp2.color[1] +
               (gamma / rz3) * sp3.color[1]),
          255 * rz *
              ((alpha / rz1) * sp1.color[2] + (beta / rz2) * sp2.color[2] +
               (gamma / rz3) * sp3.color[2]));
      } else {
        float u = rz * ((alpha / rz1) * sp1.texcoord[0] +
                      (beta / rz2) * sp2.texcoord[0] +
                      (gamma / rz3) * sp3.texcoord[0]);
        float v = rz * ((alpha / rz1) * sp1.texcoord[1] +
                      (beta / rz2) * sp2.texcoord[1] +
                      (gamma / rz3) * sp3.texcoord[1]);
        auto color = getTextureColor(texture, u * texture->w, (1 - v) * texture->h);
        mixed_color = SDL_MapRGB(surface_->format, color.r, color.g, color.b);
      }
      pixels[cx + cy * surface_->w] = mixed_color;
    }
  }
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

float Renderer::caculateArea(float a_x, float a_y, float b_x, float b_y,
                             float c_x, float c_y) {
  float area = (a_x - c_x) * (b_y - c_y) - (a_y - c_y) * (b_x - c_x);
  return std::fabs(area);
}

}  // namespace sf
