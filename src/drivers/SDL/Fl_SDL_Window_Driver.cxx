//
// Definition of SDL window driver for the Fast Light Tool Kit (FLTK).
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
#include <FL/Fl.H>
#include <FL/platform.H>
#include "Fl_SDL_Window_Driver.H"
#include "Fl_SDL_Screen_Driver.H"
#include "Fl_SDL_Graphics_Driver.H"


Fl_Window_Driver *Fl_Window_Driver::newWindowDriver(Fl_Window *w)
{
  return new Fl_SDL_Window_Driver(w);
}

Fl_SDL_Window_Driver::Fl_SDL_Window_Driver(Fl_Window* w)
: Fl_Window_Driver(w)
{
  printf("SDL Window Driver created\n");
}

Fl_SDL_Window_Driver::~Fl_SDL_Window_Driver()
{
  printf("SDL Window Driver destroyed\n");
}

void Fl_SDL_Window_Driver::show()
{
  if (!shown()) {
    makewindow();
  } else {
    // TODO: raise window
  }
}

Fl_X *Fl_SDL_Window_Driver::makewindow()
{
  Fl_Group::current(0);
  fl_open_display();

  // TODO: a lot more need to go in here, like managing subwindws, etc. etc.

  Fl_Window *w = pWindow;

  Fl_X *x = new Fl_X;
  x->w = w;
  i(x);
  x->xid = ::malloc(16); // TODO: just some empty space for now

  x->next = Fl_X::first;
  Fl_X::first = x;

  int old_event = Fl::e_number;
  w->handle(Fl::e_number = FL_SHOW); // get child windows to appear
  Fl::e_number = old_event;
  w->redraw(); // force draw to happen

  return x;
}

void Fl_SDL_Window_Driver::hide() {
  Fl_X* ip = Fl_X::i(pWindow);
  if ( hide_common() ) return;
  if ( ip->xid == fl_window )
    fl_window = 0;
  ::free(ip);
}

