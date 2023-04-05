//
// Widget screenshot header-only file for the Fast Light Tool Kit (FLTK).
//
// Copyright 2023 by Bill Spitzak and others.
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

/**
  \file fl_screenshot.H
  \brief header-only file to make screen shots of UI elements for documentation
*/

#ifndef fl_screenshot_H
#define fl_screenshot_H

#include <FL/Enumerations.H>
#include <FL/Fl_Export.H>
#include <FL/Fl_Image_Surface.H>
#include <FL/Fl_Window.H>

/** \addtogroup fl_drawings
    @{
*/

/**
 Take a snapshot of a given widget and save it as a png image.

 If a frame size is set, the area surrunding the widget is also drawn.
 If a scale is set, the snapshot is scaled in scaled in size before it is saved.

 \param[in] filename the snapshot will be written in png format to this file
 \param[in] w draw this widget
 \param[in] frame draw a half transparent frame around the snapshot, defaults to 0
 \param[in] scale scale the snapshot in size before saving it

 \note if a widget has an outside label, we must snap that as well
 \note we must draw the top level window and possibly subwindows
 \note we must show all parents, all the way to the top level window
 \note we should allow frame widths for all sides
 \note we should allow a list of widgets to be snapped
 \note we must allow snapping of menu items
 \note do we really want that transparency thing? Or is a solid border enough?
*/

//FL_EXPORT inline int fl_snapshot(const char *filename, Fl_Widget *w, int frame=0, int alphaframe=0, double scale=1.0)
//FL_EXPORT inline int fl_snapshot(const char *filename, Fl_Widget **w, int frame=0, int alphaframe=0, double scale=1.0)
//FL_EXPORT inline int fl_snapshot(const char *filename, Fl_Widget *w, int frame=0, double scale=1.0) {

static int min(int a, int b) { return a < b ? a : b; }
static int max(int a, int b) { return a > b ? a : b; }

FL_EXPORT inline int fl_snapshot(const char *filename, Fl_Widget **w, int fx, int fy, int fr, int fb, int alpha=10, double scale=1.0) {
  int i, j, c, min_x, min_y, max_x, max_y, bb_w, bb_h, img_w, img_h;

  // Get the bounding box for all widgets and make sure that all widgets are shown
  for (i=0; w[i]; i++) {
    int x, y;
    Fl_Widget *ww = w[i];
    ww->top_window_offset(x, y);
    if (i==0) {
      min_x = x; max_x = x + ww->w();
      min_y = y; max_y = y + ww->h();
    } else {
      min_x = min(min_x, x); max_x = max(max_x, x + ww->w());
      min_y = min(min_y, y); max_y = max(max_y, y + ww->h());
    }

    // this does not help us with Fl_Tab groups
    while (ww) { ww->show(); ww = ww->parent(); }
  }
  min_x -= fx; max_x += fr;
  min_y -= fy; max_y += fb;
  bb_w = max_x - min_x; bb_h = max_y - min_y;
  img_w = bb_w + 2 * alpha; img_h = bb_h + 2 * alpha;

  // Generate the Image Surface
  Fl_Image_Surface *srfc = new Fl_Image_Surface(img_w, img_h);
  Fl_Image_Surface::push_current(srfc);

  // Draw the background
  fl_rectf(0, 0, img_w, img_h, 0x1395bf00);

  // Draw the top level window
  srfc->draw(w[0]->top_window(), alpha-min_x, alpha-min_y);
  Fl_Image_Surface::pop_current();
  Fl_RGB_Image *img = srfc->image();

  // Do we want an alpha blended frame?
  if (alpha && (img->d() == 3)) {

    // If alpha is set, create the alpha channel at 50%
    uchar *data = new uchar[img_w * img_h * 4], *dst = data;
    int ld = img->ld(); if (ld == 0) ld = img_w * 3;
    int c, i, j;
    for (i=0; i<img_h; i++) {
      const uchar *src = (const uchar*)img->data()[0] + i * ld;
      for (j=0; j<img_w; j++) {
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = *src++;
        *dst++ = 128;
      }
    }

    // Now set the bounding box of all widget plus frame to 100%
    for (i=0; i<bb_h; i++) {
      dst = data + img_w * 4 * (alpha+i) + alpha * 4;
      for (j=0; j<bb_w; j++) {
        dst[3] = 255; dst += 4;
      }
    }
    Fl_RGB_Image *img4 = new Fl_RGB_Image(data, img->w(), img->h(), 4);
    delete img;
    img = img4;
  }

  // If scale is set, scale the image
  if (scale != 1.0) {
    Fl_Image::scaling_algorithm(FL_RGB_SCALING_BILINEAR);
    Fl_RGB_Image *scaled_img = (Fl_RGB_Image*)img->copy(img->w()*scale, img->h()*scale);
    delete img;
    img = scaled_img;
  }

  // Write the image to disk
  int ret = fl_write_png(filename, img);

  // Clean up
  delete img;
  delete srfc;
  return ret;
}

FL_EXPORT inline int fl_snapshot(const char *filename, Fl_Widget *w1, Fl_Widget *w2, int fx=4, int fy=4, int fr=4, int fb=4, int alpha=8, double scale=1.0) {
  Fl_Widget *ww[3] = { w1, w2, NULL };
  return fl_snapshot(filename, ww, fx, fy, fr, fb, alpha, scale);
}

FL_EXPORT inline int fl_snapshot(const char *filename, Fl_Widget *w, int frame=20, int alpha=10, double scale=1.0) {
  Fl_Widget *ww[2] = { w, NULL };
  return fl_snapshot(filename, ww, frame, frame, frame, frame, alpha, scale);
}


/** @} */

#endif
