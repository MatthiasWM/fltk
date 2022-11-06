#include <config.h>
#include "Fl_SDL_Screen_Driver.H"
#include <FL/Fl.H>
#include <FL/platform.H>

Fl_SDL_Screen_Driver::Fl_SDL_Screen_Driver() {
}

void Fl_SDL_Screen_Driver::open_display()
{
  SDL_Init(SDL_INIT_VIDEO);
  // TODO: SDL: there is a lot more to it
  SDL_Window *window = SDL_CreateWindow(
                                        "SDL2Test",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        640,
                                        480,
                                        0
                                        );

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);

  SDL_Event event;

  bool done = false;
  while((!done) && (SDL_WaitEvent(&event))) {
    switch(event.type) {
      case SDL_USEREVENT:
//        HandleUserEvents(&event);
        break;

      case SDL_KEYDOWN:
        // Handle any key presses here.
        break;

      case SDL_MOUSEBUTTONDOWN:
        // Handle mouse clicks here.
        break;

      case SDL_QUIT:
        done = true;
        break;

      default:
        break;
    }   // End switch

  }   // End while

//  SDL_Delay(3000);

  SDL_DestroyWindow(window);
  SDL_Quit();
}
