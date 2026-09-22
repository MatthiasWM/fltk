//
// macOS-specific code to initialize macOS support.
//
// Copyright 2022 by Bill Spitzak and others.
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


#include "../../Fl_Driver_Set.H"
#include "../Quartz/Fl_Quartz_Copy_Surface_Driver.H"
#include "../Quartz/Fl_Quartz_Graphics_Driver.H"
#include "../Cocoa/Fl_Cocoa_Screen_Driver.H"
#include "../Darwin/Fl_Darwin_System_Driver.H"
#include "../Cocoa/Fl_Cocoa_Window_Driver.H"
#include "../Quartz/Fl_Quartz_Image_Surface_Driver.H"


class Fl_Cocoa_Driver_Set : public Fl_Driver_Set {
public:
  Fl_Cocoa_Driver_Set() : Fl_Driver_Set("cocoa") { }

  Fl_Copy_Surface_Driver *create_copy_surface_driver(int w, int h) override {
    return new Fl_Quartz_Copy_Surface_Driver(w, h);
  }

  Fl_Graphics_Driver *create_main_graphics_driver() override {
    return new Fl_Quartz_Graphics_Driver();
  }

  Fl_Screen_Driver *create_screen_driver() override {
    return new Fl_Cocoa_Screen_Driver();
  }

  Fl_System_Driver *create_system_driver() override {
    return new Fl_Darwin_System_Driver();
  }

  Fl_Window_Driver *create_window_driver(Fl_Window *w) override {
    return new Fl_Cocoa_Window_Driver(w);
  }

  Fl_Image_Surface_Driver *create_image_surface_driver(int w, int h, int high_res, Fl_Offscreen off) override {
    return new Fl_Quartz_Image_Surface_Driver(w, h, high_res, off);
  }
};

Fl_Driver_Set *Fl_Driver_Set::make() {
  static Fl_Cocoa_Driver_Set inst;
  return &inst;
}
