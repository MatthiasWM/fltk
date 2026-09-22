//
// X11-specific code to initialize wayland support.
//
// Copyright 2022-2026 by Bill Spitzak and others.
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
#include "../Xlib/Fl_Xlib_Copy_Surface_Driver.H"
#if FLTK_USE_CAIRO
#  include "../Cairo/Fl_X11_Cairo_Graphics_Driver.H"
#else
#  include "../Xlib/Fl_Xlib_Graphics_Driver.H"
#endif
#include "Fl_X11_Screen_Driver.H"
#include "../Unix/Fl_Unix_System_Driver.H"
#include "Fl_X11_Window_Driver.H"
#include "../Xlib/Fl_Xlib_Image_Surface_Driver.H"
#include "../Base/Fl_Base_Pen_Driver.H"


class Fl_X11_Driver_Set : public Fl_Driver_Set {
public:
  Fl_X11_Driver_Set() : Fl_Driver_Set("x11") { }

  Fl_Copy_Surface_Driver *create_copy_surface_driver(int w, int h) override {
    return new Fl_Xlib_Copy_Surface_Driver(w, h);
  }

  Fl_Graphics_Driver *create_main_graphics_driver() override {
#if FLTK_USE_CAIRO
    return new Fl_X11_Cairo_Graphics_Driver();
#else
    return new Fl_Xlib_Graphics_Driver();
#endif
  }

  Fl_Screen_Driver *create_screen_driver() override {
    Fl_X11_Screen_Driver *d = new Fl_X11_Screen_Driver();
#if USE_XFT || FLTK_USE_CAIRO
    for (int i = 0;  i < Fl_Screen_Driver::MAX_SCREENS; i++) d->screens[i].scale = 1;
    d->current_xft_dpi = 0.; // means the value of the Xft.dpi resource is still unknown
#else
    secret_input_character = '*';
#endif
    return d;
  }

  Fl_System_Driver *create_system_driver() override {
    return new Fl_Unix_System_Driver();
  }

  Fl_Window_Driver *create_window_driver(Fl_Window *w) override {
    return new Fl_X11_Window_Driver(w);
  }

  Fl_Image_Surface_Driver *create_image_surface_driver(int w, int h, int high_res, Fl_Offscreen off) override {
    return new Fl_Xlib_Image_Surface_Driver(w, h, high_res, off);
  }
};

Fl_Driver_Set *Fl_Driver_Set::make() {
  static Fl_X11_Driver_Set inst;
  return &inst;
}

// This defines X11 dummy driver when Wayland is not being built.
#if FLTK_HAVE_PEN_SUPPORT && !defined(FLTK_USE_WAYLAND)
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
