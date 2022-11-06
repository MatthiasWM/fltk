
#include "Fl_SDL_Window_Driver.H"

#include <FL/platform.H>

FLWindow *fl_window;

Fl_SDL_Window_Driver::Fl_SDL_Window_Driver(Fl_Window *w)
: Fl_Window_Driver(w)
{
}

Fl_SDL_Window_Driver::~Fl_SDL_Window_Driver()
{
}

