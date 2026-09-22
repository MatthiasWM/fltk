//
// Base (null) driver set: no platform, no rendering, safe to link and run.
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

#include <config.h>
#include <FL/platform.H>
#include "../../Fl_Driver_Set.H"
#include "Fl_Base_Screen_Driver.H"
#include "Fl_Base_System_Driver.H"
#include "Fl_Base_Window_Driver.H"
#include "Fl_Base_Graphics_Driver.H"
#include "Fl_Base_Copy_Surface_Driver.H"
#include "Fl_Base_Image_Surface_Driver.H"
#if FLTK_HAVE_PEN_SUPPORT
#include "Fl_Base_Pen_Driver.H"
#endif


// Every platform's own core integration file (Fl_cocoa.mm, Fl_x.cxx, ...)
// defines this global, referenced unconditionally by common code such as
// fl_read_image(). It's not part of Fl_Driver_Set's six factories, but is
// needed here for the same link-completeness reason as newPenDriver() below.
Window fl_window;

class Fl_Base_Driver_Set : public Fl_Driver_Set {
public:
  Fl_Base_Driver_Set() : Fl_Driver_Set("base") { }

  Fl_Screen_Driver *create_screen_driver() override {
    return new Fl_Base_Screen_Driver();
  }

  Fl_System_Driver *create_system_driver() override {
    return new Fl_Base_System_Driver();
  }

  Fl_Window_Driver *create_window_driver(Fl_Window *w) override {
    return new Fl_Base_Window_Driver(w);
  }

  Fl_Graphics_Driver *create_main_graphics_driver() override {
    return new Fl_Base_Graphics_Driver();
  }

  Fl_Copy_Surface_Driver *create_copy_surface_driver(int w, int h) override {
    return new Fl_Base_Copy_Surface_Driver(w, h);
  }

  Fl_Image_Surface_Driver *create_image_surface_driver(int w, int h, int high_res, Fl_Offscreen off) override {
    return new Fl_Base_Image_Surface_Driver(w, h, high_res, off);
  }
};

Fl_Driver_Set *Fl_Driver_Set::make() {
  static Fl_Base_Driver_Set inst;
  return &inst;
}

#if FLTK_HAVE_PEN_SUPPORT
namespace Fl {
  namespace Pen
  {
    Fl::Pen::Driver& newPenDriver() {
      static Driver default_driver;
      return default_driver;
    }
  }
}
#endif
