//
// Copy-to-clipboard code for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2014 by Bill Spitzak and others.
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

#ifndef Fl_SDL_Copy_Surface_H
#define Fl_SDL_Copy_Surface_H

#include <FL/Fl_Copy_Surface.H>


/**
 \cond DriverDev
 \addtogroup DriverDeveloper
 \{
 */

/**
 A base class describing the interface between FLTK and draw-to-clipboard operations.

 This class is only for internal use by the FLTK library.

 A supported platform should implement the virtual methods of this class
 in order to support drawing to the clipboard through class Fl_Copy_Surface.
 */
class Fl_SDL_Copy_Surface_Driver : public Fl_Copy_Surface_Driver {
//  friend class Fl_Copy_Surface;
public:
  Fl_SDL_Copy_Surface_Driver(int w, int h);

protected:
//  int width;
//  int height;
  virtual ~Fl_SDL_Copy_Surface_Driver();
  virtual void set_current() { }
  virtual void translate(int x, int y) { }
  virtual void untranslate() { }
//  int printable_rect(int *w, int *h);
//  /** Each platform implements this function its own way.
//   It returns an object implementing all virtual functions
//   of class Fl_Copy_Surface_Driver for the plaform.
//   */
//  static Fl_Copy_Surface_Driver *newCopySurfaceDriver(int w, int h);
};

/**
 \}
 \endcond
 */

#endif // Fl_Copy_Surface_H
