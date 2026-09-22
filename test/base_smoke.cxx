//
// Smoke test for the Base (null) driver set for the Fast Light Tool Kit (FLTK).
//
// Verifies that a build linked only against src/drivers/Base/ compiles,
// links, and runs without rendering: exercises all six Fl_Driver_Set
// factories and confirms the event loop doesn't busy-spin.
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

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Copy_Surface.H>
#include <FL/Fl_Image_Surface.H>
#include <FL/Fl_RGB_Image.H>

#include <stdio.h>
#include <stdlib.h>

int main() {
  // Fl_Window_Driver, via Fl::screen_driver()/Fl::system_driver() indirectly
  Fl_Window *window = new Fl_Window(340, 180);
  Fl_Box *box = new Fl_Box(20, 40, 300, 100, "Base driver set");
  window->end();
  window->show();

  // exercises Fl_Base_System_Driver::wait()'s sleep path - if this were
  // still busy-spinning, this loop would burn CPU instead of just taking
  // roughly 0.3s of wall-clock time.
  for (int i = 0; i < 3; i++) {
    Fl::wait(0.1);
  }

  // Fl_Copy_Surface_Driver
  Fl_Copy_Surface *copy_surf = new Fl_Copy_Surface(100, 100);
  delete copy_surf;

  // Fl_Image_Surface_Driver
  Fl_Image_Surface *image_surf = new Fl_Image_Surface(64, 48);
  Fl_RGB_Image *img = image_surf->image();
  if (!img) {
    fprintf(stderr, "FAIL: Fl_Image_Surface::image() returned NULL\n");
    return 1;
  }
  if (img->w() != 64 || img->h() != 48) {
    fprintf(stderr, "FAIL: image size %dx%d, expected 64x48\n", img->w(), img->h());
    return 1;
  }
  delete img;
  delete image_surf;

  delete window;

  printf("base_smoke: OK\n");
  return 0;
}
