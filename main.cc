#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <cstdio>
#include <chrono>

#include "renderer.h"
#include "transform.hpp"

int main() {
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window *window = SDL_CreateWindow(
      "sf-render", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800,
      SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

  SDL_Surface *surface = SDL_GetWindowSurface(window);
  SDL_Surface* texture = SDL_LoadBMP("./img/sample.bmp");
  sf::Renderer renderer(surface);
  renderer.setViewport(0, 0, 800, 800);

  sf::Matrix<float, 4> projection;
  float n = -0.9f;
  float f = -100.f;
  projection[0][0] = n;
  projection[1][1] = n;
  projection[2][2] = n + f;
  projection[2][3] = -n * f;
  projection[3][3] = 0.f;
  projection[3][2] = 1.f;

  sf::Vertex v1;
  v1.position[0] = -1.f;
  v1.position[1] = -1.f;
  v1.position[2] = 1.f;
  v1.position[3] = 1.f;

  v1.color[0] = 1.f;
  v1.color[1] = 0.f;
  v1.color[2] = 0.f;

  v1.texcoord[0] = 0.f;
  v1.texcoord[1] = 0.f;

  sf::Vertex v2;
  v2.position[0] = 1.f;
  v2.position[1] = -1.f;
  v2.position[2] = 1.f;
  v2.position[3] = 1.f;

  v2.color[0] = 1.f;
  v2.color[1] = 0.f;
  v2.color[2] = 0.f;

  v2.texcoord[0] = 1.f;
  v2.texcoord[1] = 0.f;

  sf::Vertex v3;
  v3.position[0] = -1.f;
  v3.position[1] = 1.f;
  v3.position[2] = 1.f;
  v3.position[3] = 1.f;

  v3.color[0] = 0.f;
  v3.color[1] = 1.f;
  v3.color[2] = 0.f;

  v3.texcoord[0] = 0.f;
  v3.texcoord[1] = 1.f;

  sf::Vertex v4;
  v4.position[0] = 1.f;
  v4.position[1] = 1.f;
  v4.position[2] = 1.f;
  v4.position[3] = 1.f;

  v4.color[0] = 0.f;
  v4.color[1] = 0.f;
  v4.color[2] = 1.f;

  v4.texcoord[0] = 1.f;
  v4.texcoord[1] = 1.f;

  sf::Vertex v5;
  v5.position[0] = -1.f;
  v5.position[1] = -1.f;
  v5.position[2] = -1.f;
  v5.position[3] = 1.f;

  v5.color[0] = 1.f;
  v5.color[1] = 0.f;
  v5.color[2] = 0.f;

  v5.texcoord[0] = 0.f;
  v5.texcoord[1] = 0.f;

  sf::Vertex v6;
  v6.position[0] = 1.f;
  v6.position[1] = -1.f;
  v6.position[2] = -1.f;
  v6.position[3] = 1.f;

  v6.color[0] = 1.f;
  v6.color[1] = 0.f;
  v6.color[2] = 0.f;

  v6.texcoord[0] = 1.f;
  v6.texcoord[1] = 0.f;

  sf::Vertex v7;
  v7.position[0] = -1.f;
  v7.position[1] = 1.f;
  v7.position[2] = -1.f;
  v7.position[3] = 1.f;

  v7.color[0] = 0.f;
  v7.color[1] = 1.f;
  v7.color[2] = 0.f;

  v7.texcoord[0] = 0.f;
  v7.texcoord[1] = 1.f;

  sf::Vertex v8;
  v8.position[0] = 1.f;
  v8.position[1] = 1.f;
  v8.position[2] = -1.f;
  v8.position[3] = 1.f;

  v8.color[0] = 0.f;
  v8.color[1] = 0.f;
  v8.color[2] = 1.f;

  v8.texcoord[0] = 1.f;
  v8.texcoord[1] = 1.f;

  bool is_close = false;
  SDL_Event event;
  auto current = std::chrono::system_clock::now();
  auto last = std::chrono::system_clock::now();
  while (!is_close) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        is_close = true;
      }
    }
    sf::Vector<float, 3> color;
    color[0] = 0.2f;
    color[1] = 0.4f;
    color[2] = 0.1f;
    renderer.clearColor(color);

    sf::Matrix<float, 4> transform;

    sf::Vector<float, 3> vec;
    vec[0] = 2.f;
    vec[1] = 1.f;
    vec[2] = 1.f;
  
    current = std::chrono::system_clock::now();
    std::chrono::duration<double, std::milli> duration = current - last;
    transform = sf::Transform::rotate<float>(transform, duration.count() / 1000.f, vec);

    sf::Vector<float, 3> vec2;
    vec2[0] = 0.f;
    vec2[1] = 0.f;
    vec2[2] = -5.f;

    transform = sf::Transform::translate<float>(transform, vec2);

    renderer.setTransform(projection * transform);
    // front
    renderer.fillTriangle(v1, v2, v3, nullptr);
    renderer.fillTriangle(v3, v2, v4, nullptr);
    // top
    renderer.fillTriangle(v3, v4, v7, nullptr);
    renderer.fillTriangle(v8, v7, v4, nullptr);
    // down
    renderer.fillTriangle(v1, v5, v2, nullptr);
    renderer.fillTriangle(v2, v5, v6, nullptr);
    // left
    renderer.fillTriangle(v1, v3, v7, nullptr);
    renderer.fillTriangle(v7, v5, v1, nullptr);
    //right
    renderer.fillTriangle(v2, v6, v8, nullptr);
    renderer.fillTriangle(v8, v4, v2, nullptr);
    //back
    renderer.fillTriangle(v7, v8, v6, nullptr);
    renderer.fillTriangle(v6, v5, v7, nullptr);

    SDL_UpdateWindowSurface(window);
  }
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
