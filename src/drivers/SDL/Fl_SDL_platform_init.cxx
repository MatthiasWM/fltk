


#include "Fl_SDL_Copy_Surface_Driver.H"
#include "Fl_SDL_Graphics_Driver.H"
#include "Fl_SDL_Screen_Driver.H"
#include "Fl_SDL_System_Driver.H"
#include "Fl_SDL_Window_Driver.H"
#include "Fl_SDL_Image_Surface_Driver.H"
#include <FL/Fl_Printer.H>
#include <FL/Fl_Native_File_Chooser.H>


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


Fl_Paged_Device* Fl_Printer::newPrinterDriver(void)
{
  return NULL; // TODO: SDL: new Fl_Cocoa_Printer_Driver();
}

Fl_Native_File_Chooser::Fl_Native_File_Chooser(int val) {
//  platform_fnfc = new Fl_Quartz_Native_File_Chooser_Driver(val);
}
