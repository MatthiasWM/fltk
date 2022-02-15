//
// Definition of SDL window driver
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
 \file Fl_SDL_Window_Driver.H
 \brief Definition of Windows window driver.
 */

#ifndef FL_SDL_WINDOW_DRIVER_H
#define FL_SDL_WINDOW_DRIVER_H

#include "../../Fl_Window_Driver.H"

class FL_EXPORT Fl_SDL_Window_Driver : public Fl_Window_Driver
{
public:
  Fl_SDL_Window_Driver(Fl_Window* w);
  virtual ~Fl_SDL_Window_Driver();
  virtual void show();
  virtual Fl_X *makewindow();
  virtual void hide();
};


#endif // FL_SDL_WINDOW_DRIVER_H
