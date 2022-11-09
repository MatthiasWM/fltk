#include <config.h>
#include "Fl_SDL_Screen_Driver.H"
#include <FL/Fl.H>
#include <FL/platform.H>

Fl_SDL_Screen_Driver::Fl_SDL_Screen_Driver() {
}

void Fl_SDL_Screen_Driver::get_system_colors()
{
  open_display();
  // We can also set some color map entries to make FLTK match the
  // host color scheme.
}

void Fl_SDL_Screen_Driver::open_display()
{
  open_display_platform();
  Fl_Graphics_Driver::default_driver();
  // TODO: SDL: setup scalable graphics driver
}

void Fl_SDL_Screen_Driver::open_display_platform()
{
  static char beenHereDoneThat = 0;
  if ( !beenHereDoneThat ) {
    beenHereDoneThat = 1;
    SDL_Init(SDL_INIT_VIDEO);
    // TODO: SDL: there is a lot more to it
    _screen = SDL_CreateWindow("FLTK",
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               w(),
                               h(),
                               0
                               );

    if (_screen == NULL) {
      // In the case that the window could not be made...
      printf("Could not create SDL window: %s\n", SDL_GetError());
    }

    fl_gc = SDL_CreateRenderer(_screen, -1, SDL_RENDERER_SOFTWARE);
    SDL_SetRenderDrawColor(fl_gc, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(fl_gc);
    SDL_RenderPresent(fl_gc);
  }
}

void Fl_SDL_Screen_Driver::flush()
{
  // This is called after every burst of events by void Fl::flush()
  // TODO: SDL: if nothing changed, there is no need to flush anything, right?
  SDL_RenderPresent(fl_gc);
}
