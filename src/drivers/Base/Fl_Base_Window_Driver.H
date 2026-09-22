//
// Definition of default (no-op) window driver.
//
// Copyright 2010-2026 by Bill Spitzak and others.
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

#ifndef FL_BASE_WINDOW_DRIVER_H
#define FL_BASE_WINDOW_DRIVER_H

#include "../../Fl_Window_Driver.H"

/**
 \brief The default Window driver: does no work, but is safe to use standalone.

 Fl_Window_Driver's own defaults (show(), make_current(), label() are all
 empty; wait_for_expose() returns immediately since a window is never
 actually mapped) are already exactly what's needed here. This class only
 exists so Fl_Base_Driver_Set has a self-documenting, named type to return,
 matching Fl_Cocoa_Window_Driver etc.
*/
class Fl_Base_Window_Driver : public Fl_Window_Driver {
public:
  Fl_Base_Window_Driver(Fl_Window *w) : Fl_Window_Driver(w) {}
};

#endif // FL_BASE_WINDOW_DRIVER_H
