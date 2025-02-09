#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <cstdio>

#include "renderer.h"

int main() {
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window *window = SDL_CreateWindow(
      "sf-render", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
      SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

  SDL_Surface *surface = SDL_GetWindowSurface(window);
  SDL_Surface* texture = SDL_LoadBMP("./img/sample.bmp");
  sf::Renderer renderer(surface);
  renderer.setViewport(0, 0, 800, 600);
  sf::Matrix<float, 4> transform;
  float n = -0.9f;
  float f = -100.f;
  transform[0][0] = n;
  transform[1][1] = n;
  transform[2][2] = n + f;
  transform[2][3] = -n * f;
  transform[3][3] = 0.f;
  transform[3][2] = 1.f;

  renderer.setTransform(transform);
  sf::Vertex v1;
  v1.position[0] = -0.5f;
  v1.position[1] = -0.5f;
  v1.position[2] = -1.f;
  v1.position[3] = 1.f;

  v1.color[0] = 1.f;
  v1.color[1] = 0.f;
  v1.color[2] = 0.f;

  v1.texcoord[0] = 0.f;
  v1.texcoord[1] = 0.f;

  sf::Vertex v2;
  v2.position[0] = 0.5f;
  v2.position[1] = -0.5f;
  v2.position[2] = -1.f;
  v2.position[3] = 1.f;

  v2.color[0] = 1.f;
  v2.color[1] = 0.f;
  v2.color[2] = 0.f;

  v2.texcoord[0] = 1.f;
  v2.texcoord[1] = 0.f;

  sf::Vertex v3;
  v3.position[0] = -0.5f;
  v3.position[1] = 0.5f;
  v3.position[2] = -1.f;
  v3.position[3] = 1.f;

  v3.color[0] = 0.f;
  v3.color[1] = 1.f;
  v3.color[2] = 0.f;

  v3.texcoord[0] = 0.f;
  v3.texcoord[1] = 1.f;

  sf::Vertex v4;
  v4.position[0] = 0.5f;
  v4.position[1] = 0.5f;
  v4.position[2] = -1.f;
  v4.position[3] = 1.f;

  v4.color[0] = 0.f;
  v4.color[1] = 0.f;
  v4.color[2] = 1.f;

  v4.texcoord[0] = 1.f;
  v4.texcoord[1] = 1.f;

  renderer.fillTriangle(v1, v2, v3, texture);
  renderer.fillTriangle(v3, v2, v4, texture);
  bool is_close = false;
  SDL_Event event;
  while (!is_close) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        is_close = true;
      }
    }
    SDL_UpdateWindowSurface(window);
  }
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
