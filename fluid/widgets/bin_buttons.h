//
// Widget Bin Button Classes for Fast Light User Interface Designer (FLUID).
//
// Copyright 1998-2024 by Bill Spitzak and others.
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

#ifndef FLUID_WIDGETS_BIN_BUTTONS_H
#define FLUID_WIDGETS_BIN_BUTTONS_H

#include <FL/Fl_Button.H>

namespace fluid {

namespace widget {

// Adding drag and drop for dragging widgets into windows.
class BinButton : public Fl_Button {
public:
  int handle(int) FL_OVERRIDE;
  BinButton(int X,int Y,int W,int H, const char* l = 0) :
  Fl_Button(X,Y,W,H,l) { }
};

// Adding drag and drop functionality to drag window prototypes onto the desktop.
class BinWindowButton : public Fl_Button {
public:
  int handle(int) FL_OVERRIDE;
  BinWindowButton(int X,int Y,int W,int H, const char* l = 0) :
  Fl_Button(X,Y,W,H,l) { }
};

} // namespace widget

} // namespace fluid

#endif // FLUID_WIDGETS_BIN_BUTTONS_H
