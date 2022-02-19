//
// SDL drawing routines for the Fast Light Tool Kit (FLTK).
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


#include <config.h>
#include "Fl_SDL_Graphics_Driver.H"
#include <FL/Fl.H>
#include <FL/platform.H>
#include <FL/fl_draw.H>
#include "../../Fl_Screen_Driver.H"


SDL_Renderer *fl_gc = 0L;

/*
 * By linking this module, the following static method will instantiate the
 * SDL Graphics driver as the main display driver.
 */
Fl_Graphics_Driver *Fl_Graphics_Driver::newMainGraphicsDriver()
{
  return new Fl_SDL_Graphics_Driver();
}

Fl_SDL_Graphics_Driver::Fl_SDL_Graphics_Driver()
: Fl_Graphics_Driver()
{
  printf("SDL Graphics Driver created\n");
}

Fl_SDL_Graphics_Driver::~Fl_SDL_Graphics_Driver()
{
  printf("SDL Graphics Driver destroyed\n");
}

void Fl_SDL_Graphics_Driver::color(Fl_Color c)
{
  color_ = c;
  uchar r, g, b;
  Fl::get_color(c, r, g, b);
  SDL_SetRenderDrawColor(fl_gc, r, g, b, SDL_ALPHA_OPAQUE);
}

void Fl_SDL_Graphics_Driver::rect(int x, int y, int w, int h)
{
  SDL_Rect rect = { x, y, w, h };
  SDL_RenderDrawRect(fl_gc, &rect);
}

void Fl_SDL_Graphics_Driver::rectf(int x, int y, int w, int h)
{
  SDL_Rect rect = { x, y, w, h };
  SDL_RenderFillRect(fl_gc, &rect);
}
