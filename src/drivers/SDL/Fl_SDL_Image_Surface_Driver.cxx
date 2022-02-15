
//
// Draw-to-image code for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2022 by Bill Spitzak and others.
//
// This library is free software. Distribution and use rights are outlined in
// the file "COPYING" which should have been included with this file.  If this
// file is missing or damaged, see the license at:
//
//     https://www.fltk.org/COPYING.php
//
// Please see the following page on how to report bugs and issues:
//
//     https://www.fltk.org/bugs.php
//


#include "Fl_SDL_Graphics_Driver.H"
#include "Fl_SDL_Screen_Driver.H"
#include <FL/Fl_Image_Surface.H>
#include <FL/platform.H>

class Fl_SDL_Image_Surface_Driver : public Fl_Image_Surface_Driver {
public:
  Fl_SDL_Image_Surface_Driver(int w, int h, int high_res, Fl_Offscreen off);
  virtual void set_current() { }
  virtual void translate(int x, int y) { }
  virtual void untranslate() { }
  virtual Fl_RGB_Image *image() { return 0L; }
};


Fl_Image_Surface_Driver *Fl_Image_Surface_Driver::newImageSurfaceDriver(int w, int h, int high_res, Fl_Offscreen off)
{
  return new Fl_SDL_Image_Surface_Driver(w, h, high_res, off);
}


Fl_SDL_Image_Surface_Driver::Fl_SDL_Image_Surface_Driver(int w, int h, int high_res, Fl_Offscreen off) : Fl_Image_Surface_Driver(w, h, high_res, off) {
}

