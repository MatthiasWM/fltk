


#include "Fl_SDL_Copy_Surface_Driver.H"
#include "Fl_SDL_Graphics_Driver.H"
#include "Fl_SDL_Screen_Driver.H"
#include "Fl_SDL_System_Driver.H"
#include "Fl_SDL_Window_Driver.H"
#include "Fl_SDL_Image_Surface_Driver.H"

Fl_Copy_Surface_Driver *Fl_Copy_Surface_Driver::newCopySurfaceDriver(int w, int h)
{
  return new Fl_SDL_Copy_Surface_Driver(w, h);
}


Fl_Graphics_Driver *Fl_Graphics_Driver::newMainGraphicsDriver()
{
  return new Fl_SDL_Graphics_Driver();
}


Fl_Screen_Driver *Fl_Screen_Driver::newScreenDriver()
{
  return new Fl_SDL_Screen_Driver();
}


Fl_System_Driver *Fl_System_Driver::newSystemDriver()
{
  return new Fl_SDL_System_Driver();
}


Fl_Window_Driver *Fl_Window_Driver::newWindowDriver(Fl_Window *w)
{
  return new Fl_SDL_Window_Driver(w);
}


Fl_Image_Surface_Driver *Fl_Image_Surface_Driver::newImageSurfaceDriver(int w, int h, int high_res, Fl_Offscreen off)
{
  return new Fl_SDL_Image_Surface_Driver(w, h, high_res, off);
}
