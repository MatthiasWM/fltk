//
// Definition of default (no-op) screen driver.
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

#ifndef FL_BASE_SCREEN_DRIVER_H
#define FL_BASE_SCREEN_DRIVER_H

#include "../../Fl_Screen_Driver.H"

/**
 \brief The default Screen driver: does no work, but is safe to use standalone.

 Used when no platform-specific screen driver is linked in. All drawing
 and windowing calls are no-ops; the base class already provides safe
 default bodies for everything except num_screens, which this class fixes
 up so Fl_Screen_Driver::screen_count() reports 1 instead of -1.
*/
class Fl_Base_Screen_Driver : public Fl_Screen_Driver {
public:
  Fl_Base_Screen_Driver() { num_screens = 1; }
};

#endif // FL_BASE_SCREEN_DRIVER_H
