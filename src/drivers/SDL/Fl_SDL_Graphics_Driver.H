//
// Definition of classes  Fl_Graphics_Driver, Fl_Surface_Device, Fl_Display_Device
// for the Fast Light Tool Kit (FLTK).
//
// Copyright 2010-2022 by Bill Spitzak and others.
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

/**
 \file Fl_SDL_Graphics_Driver.H
 \brief Definition of SDL graphics driver.
 */

#ifndef FL_SDL_GRAPHICS_DRIVER_H
#define FL_SDL_GRAPHICS_DRIVER_H

#include <FL/Fl_Graphics_Driver.H>

/**
 \brief The SDL graphics driver class.

 This class is implemented only on the Windows platform.
 */
class FL_EXPORT Fl_SDL_Graphics_Driver : public Fl_Graphics_Driver {
public:
  Fl_SDL_Graphics_Driver();
  virtual ~Fl_SDL_Graphics_Driver();
  virtual void color(Fl_Color c);
  virtual void rect(int x, int y, int w, int h);
  virtual void rectf(int x, int y, int w, int h);
  virtual void xyline(int x, int y, int x1);
  virtual void xyline(int x, int y, int x1, int y1);
  virtual void xyline(int x, int y, int x1, int y1, int x2);
  virtual void yxline(int x, int y, int y1);
  virtual void yxline(int x, int y, int y1, int x1);
  virtual void yxline(int x, int y, int y1, int x1, int y2);
};

#endif // FL_SDL_GRAPHICS_DRIVER_H
