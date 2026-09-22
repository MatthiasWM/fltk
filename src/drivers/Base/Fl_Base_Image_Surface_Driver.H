//
// Definition of default (no-op) image-surface driver.
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

#ifndef FL_BASE_IMAGE_SURFACE_DRIVER_H
#define FL_BASE_IMAGE_SURFACE_DRIVER_H

#include <FL/Fl_Image_Surface.H>
#include "../../Fl_Driver_Set.H"

/**
 \brief The default Image_Surface driver: does no work, but is safe to use standalone.

 Acquires a fresh Fl_Base_Graphics_Driver the same way every real platform's
 Image_Surface_Driver acquires a fresh real one, and frees it again on
 destruction. image() returns a real, correctly sized but blank image
 rather than NULL, so callers that inspect it don't crash.
*/
class Fl_Base_Image_Surface_Driver : public Fl_Image_Surface_Driver {
public:
  Fl_Base_Image_Surface_Driver(int w, int h, int high_res, Fl_Offscreen off)
  : Fl_Image_Surface_Driver(w, h, high_res, off) {
    driver(Fl_Driver_Set::current()->create_main_graphics_driver());
  }
  ~Fl_Base_Image_Surface_Driver() { delete driver(); }
  void set_current() override { Fl_Surface_Device::set_current(); }
  void translate(int /*x*/, int /*y*/) override {}
  void untranslate() override {}
  Fl_RGB_Image *image() override {
    uchar *data = new uchar[width * height * 3](); // zero-initialized
    Fl_RGB_Image *img = new Fl_RGB_Image(data, width, height, 3);
    img->alloc_array = 1;
    return img;
  }
};

#endif // FL_BASE_IMAGE_SURFACE_DRIVER_H
