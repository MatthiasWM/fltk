
//
// SDL screen interface for the Fast Light Tool Kit (FLTK).
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
 \file Fl_SDL_Screen_Driver.H
 \brief Definition of SDL screen interface.
 */

#ifndef FL_SDL_SCREEN_DRIVER_H
#define FL_SDL_SCREEN_DRIVER_H

#include "../../Fl_Screen_Driver.H"

#include <SDL2/SDL.h>

class Fl_Window;

class FL_EXPORT Fl_SDL_Screen_Driver : public Fl_Screen_Driver
{
protected:
  SDL_Window *screen_;
  SDL_Renderer *renderer_;
  SDL_Texture *texture_;
public:
  Fl_SDL_Screen_Driver();
  virtual ~Fl_SDL_Screen_Driver();

  virtual void open_display_platform();
  virtual void close_display();
  virtual void get_system_colors();
  virtual void flush();
};


#endif // FL_SDL_SCREEN_DRIVER_H
