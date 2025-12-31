//
// Repeat button widget for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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

#include <fltk3/Fl.H>
#include <fltk3/Fl_Repeat_Button.H>

#define INITIALREPEAT .5
#define REPEAT .1

void Fl_Repeat_Button::repeat_callback(void *v) {
  fltk3::Button *b = (fltk3::Button*)v;
  Fl::add_timeout(REPEAT,repeat_callback,b);
  b->do_callback(FL_REASON_RESELECTED);
}

int Fl_Repeat_Button::handle(int event) {
  int newval;
  switch (event) {
  case fltk3::HIDE:
  case fltk3::DEACTIVATE:
  case fltk3::RELEASE:
    newval = 0; goto J1;
  case fltk3::PUSH:
  case fltk3::DRAG:
    if (Fl::visible_focus()) Fl::focus(this);
    newval = Fl::event_inside(this);
  J1:
    if (!active())
      newval = 0;
    if (value(newval)) {
      if (newval) {
        Fl::add_timeout(INITIALREPEAT,repeat_callback,this);
        do_callback(FL_REASON_SELECTED);
      } else {
        Fl::remove_timeout(repeat_callback,this);
      }
    }
    return 1;
  default:
    return fltk3::Button::handle(event);
  }
}


Fl_Repeat_Button::Fl_Repeat_Button(int X,int Y,int W,int H,const char *l)
: fltk3::Button(X,Y,W,H,l)
{
}
