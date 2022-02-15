//
// SDL screen interface for the Fast Light Tool Kit (FLTK).
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
#include "Fl_SDL_Screen_Driver.H"
#include <FL/Fl.H>
#include <FL/platform.H>
#include "Fl_SDL_Graphics_Driver.H"
#include <FL/Fl_RGB_Image.H>
#include <FL/fl_ask.H>
#include <stdio.h>

#include <SDL2/SDL.h>


/*
 Creates a driver that manages all screen and display related calls.
 This function must be implemented once for every platform.
 */
Fl_Screen_Driver *Fl_Screen_Driver::newScreenDriver()
{
  return new Fl_SDL_Screen_Driver();
}

Fl_SDL_Screen_Driver::Fl_SDL_Screen_Driver()
: Fl_Screen_Driver(),
screen_(0L),
renderer_(0L)
{
  printf("SDL Screen Driver created\n");
}

Fl_SDL_Screen_Driver::~Fl_SDL_Screen_Driver()
{
  printf("SDL Screen Driver destroyed\n");
  close_display();
}

// implement to open access to the display
void Fl_SDL_Screen_Driver::open_display_platform()
{
  if (!screen_) {
    screen_ = SDL_CreateWindow("FLTK",
                               SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               1024, 768,
                               /* flags */ 0);
    // TODO: error handling
  }
  if (!renderer_) {
    // try an accelerated renderer first
    renderer_ = SDL_CreateRenderer(screen_, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer_) {
      // if that fails, try a software renderer
      renderer_ = SDL_CreateRenderer(screen_, -1, SDL_RENDERER_SOFTWARE);
      // TODO: error handling
    }
  }
}

// optional method to close display access
void Fl_SDL_Screen_Driver::close_display()
{
  if (renderer_) {
    SDL_DestroyRenderer(renderer_);
    renderer_ = 0L;
  }
  if (screen_) {
    SDL_DestroyWindow(screen_);
    screen_ = 0L;
  }
}


void Fl_SDL_Screen_Driver::get_system_colors()
{
  open_display();
  // Don't do any color changes yet.
}
