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
  renderer.drawLine(sf::Point<int>(-50, -50), sf::Point<float>(0., 0.), sf::Point<int>(800, 400),
                    sf::Point<float>(1., 1.), texture);
  renderer.drawTriangle(sf::Point<int>(100, 0), sf::Color{255, 0, 0},
                        sf::Point<int>(0, 100), sf::Color{0, 255, 0},
                        sf::Point<int>(200, 200), sf::Color{0, 0, 255});
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
