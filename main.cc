#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>

#include "renderer.h"

int main() {
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window *window = SDL_CreateWindow(
      "sf-render", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
      SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

  SDL_Surface *surface = SDL_GetWindowSurface(window);
  sf::Renderer renderer(surface);
  renderer.drawLine(sf::Point(-50, -50), sf::Color{.r = 0, .g = 255, .b = 0},
                    sf::Point(800, 400), sf::Color{.r = 255, .g = 0, .b = 0});

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
