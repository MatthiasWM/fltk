//
// SDL system driver for the Fast Light Tool Kit (FLTK).
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
 \file Fl_SDL_System_Driver.H
 \brief Definition of SDL system driver.
 */

#ifndef FL_SDL_SYSTEM_DRIVER_H
#define FL_SDL_SYSTEM_DRIVER_H

#include "../../Fl_System_Driver.H"

#include <SDL2/SDL.h>

#include <stdarg.h>
#include <string.h>  // strdup

class Fl_SDL_System_Driver : public Fl_System_Driver
{
public:
  Fl_SDL_System_Driver();
  ~Fl_SDL_System_Driver();
  virtual double wait(double time_to_wait);
};

#endif // FL_SDL_SYSTEM_DRIVER_H
