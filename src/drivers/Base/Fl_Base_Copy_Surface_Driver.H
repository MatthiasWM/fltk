//
// Definition of default (no-op) copy-surface driver.
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

#ifndef FL_BASE_COPY_SURFACE_DRIVER_H
#define FL_BASE_COPY_SURFACE_DRIVER_H

#include <FL/Fl_Copy_Surface.H>
#include "../../Fl_Driver_Set.H"

/**
 \brief The default Copy_Surface driver: does no work, but is safe to use standalone.

 Acquires a fresh Fl_Base_Graphics_Driver the same way every real platform's
 Copy_Surface_Driver acquires a fresh real one (surfaces don't share the
 app's main graphics driver), and frees it again on destruction.
*/
class Fl_Base_Copy_Surface_Driver : public Fl_Copy_Surface_Driver {
public:
  Fl_Base_Copy_Surface_Driver(int w, int h) : Fl_Copy_Surface_Driver(w, h) {
    driver(Fl_Driver_Set::current()->create_main_graphics_driver());
  }
  ~Fl_Base_Copy_Surface_Driver() { delete driver(); }
  void set_current() override { Fl_Surface_Device::set_current(); }
  void translate(int /*x*/, int /*y*/) override {}
  void untranslate() override {}
};

#endif // FL_BASE_COPY_SURFACE_DRIVER_H
