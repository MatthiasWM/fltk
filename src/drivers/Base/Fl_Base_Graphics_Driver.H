//
// Definition of default (no-op) graphics driver.
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

#ifndef FL_BASE_GRAPHICS_DRIVER_H
#define FL_BASE_GRAPHICS_DRIVER_H

#include <FL/Fl_Graphics_Driver.H>

/**
 \brief The default Graphics driver: does no work, but is safe to use standalone.

 Every one of Fl_Graphics_Driver's ~90 virtual methods already has a safe
 default body (mostly no-ops); this class only exists to expose the
 protected constructor.
*/
class Fl_Base_Graphics_Driver : public Fl_Graphics_Driver {
public:
  Fl_Base_Graphics_Driver() {}
};

#endif // FL_BASE_GRAPHICS_DRIVER_H
