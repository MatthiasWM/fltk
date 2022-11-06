//
// Draw-to-image code for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2016 by Bill Spitzak and others.
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

#ifndef Fl_SDL_Image_Surface_H
#define Fl_SDL_Image_Surface_H

#include <FL/Fl_Image_Surface.H>
//#include <FL/Fl_Image.H>
//#include <FL/Fl_Shared_Image.H>
//#include <FL/platform_types.h> // for Fl_Offscreen


/**
 \cond DriverDev
 \addtogroup DriverDeveloper
 \{
 */

/** A base class describing the interface between FLTK and draw-to-image operations.
 This class is only for internal use by the FLTK library.
 A supported platform should implement the virtual methods of this class
 in order to support drawing to an Fl_RGB_Image through class Fl_Image_Surface.
 */
class Fl_SDL_Image_Surface_Driver : public Fl_Image_Surface_Driver {
//  friend class Fl_Image_Surface;
private:
//  Fl_Image_Surface *image_surface_;
public:
  Fl_SDL_Image_Surface_Driver(int w, int h, int /*high_res*/, Fl_Offscreen off);
protected:
//  int width;
//  int height;
//  Fl_Offscreen offscreen;
//  int external_offscreen;
//  virtual ~Fl_Image_Surface_Driver() {}
  virtual void set_current() { }
  virtual void translate(int x, int y) { }
  virtual void untranslate() { }
//  int printable_rect(int *w, int *h);
  virtual Fl_RGB_Image *image() { return NULL; }
//  /** Each platform implements this function its own way.
//   It returns an object implementing all virtual functions
//   of class Fl_Image_Surface_Driver for the plaform.
//   */
//  static Fl_Image_Surface_Driver *newImageSurfaceDriver(int w, int h, int high_res, Fl_Offscreen off);
};

/**
 \}
 \endcond
 */

#endif // Fl_Image_Surface_H
