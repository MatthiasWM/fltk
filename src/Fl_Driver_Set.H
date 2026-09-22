//
// A self-registering set of platform driver factories for the
// Fast Light Tool Kit (FLTK).
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

/**
 \cond DriverDev
 \addtogroup DriverDeveloper
 \{
 */

/** \file Fl_Driver_Set.H
 \brief declaration of class Fl_Driver_Set.
*/

#ifndef FL_DRIVER_SET_H
#define FL_DRIVER_SET_H

#include <FL/Fl_Plugin.H>
#include <FL/platform_types.h>

class Fl_Screen_Driver;
class Fl_System_Driver;
class Fl_Window_Driver;
class Fl_Window;
class Fl_Graphics_Driver;
class Fl_Copy_Surface_Driver;
class Fl_Image_Surface_Driver;

/**
 \brief A named, self-registering set of the platform driver factories.

 This class is only for internal use by the FLTK library.

 Each supported platform backend (Cocoa, X11, Wayland, WinAPI, ...) defines
 exactly one subclass, bundling the factories for that platform's
 Fl_Screen_Driver, Fl_System_Driver, Fl_Window_Driver, Fl_Graphics_Driver,
 Fl_Copy_Surface_Driver and Fl_Image_Surface_Driver, and defines the one
 out-of-line body of Fl_Driver_Set::make() that constructs it. Which
 subclass gets built in is decided by which platform's sources CMake
 compiled in, exactly as it decided which single definition of each of the
 old newXxxDriver() factories got linked.

 Registration into the Fl_Plugin database (klass(), i.e. "fltk:driverset")
 happens as a side effect of construction, inherited from Fl_Plugin. This
 is what lets a driver set loaded later - via Fl_Plugin_Manager::load(), a
 dynamically linked module - coexist with, and be selected over, the
 built-in one (see current()).

 The built-in driver set is still constructed lazily, via a function-local
 static inside current(), rather than via a self-registering *global*
 object. That is deliberate: Fl.cxx has namespace-scope globals
 (fl_local_shift, fl_local_meta, ...) whose own dynamic initializers call
 Fl::system_driver(), and C++ does not guarantee dynamic-initialization
 order across translation units - a self-registering global living in
 another translation unit could easily run after those, not before. A
 function-local static sidesteps that: it is guaranteed to initialize on
 first use, wherever that first use happens to occur. (A dynamically
 loaded driver set does not have this problem: dlopen()/LoadLibrary() runs
 well after main() and after every static initializer in the main program,
 so it may self-register via an ordinary global instance.)
*/
class Fl_Driver_Set : public Fl_Plugin {
  const char *name_;
  static Fl_Driver_Set *make();
protected:
  Fl_Driver_Set(const char *name) : Fl_Plugin(klass(), name), name_(name) {}
public:
  /** The Fl_Plugin klass under which all driver sets register themselves. */
  static const char *klass() { return "fltk:driverset"; }
  const char *name() const { return name_; }
  virtual Fl_Screen_Driver *create_screen_driver() = 0;
  virtual Fl_System_Driver *create_system_driver() = 0;
  virtual Fl_Window_Driver *create_window_driver(Fl_Window *w) = 0;
  virtual Fl_Graphics_Driver *create_main_graphics_driver() = 0;
  virtual Fl_Copy_Surface_Driver *create_copy_surface_driver(int w, int h) = 0;
  virtual Fl_Image_Surface_Driver *create_image_surface_driver(int w, int h, int high_res, Fl_Offscreen off) = 0;
  /** Return the active driver set, constructing the built-in one on first call. */
  static Fl_Driver_Set *current();
};

#endif // FL_DRIVER_SET_H

/**
 \}
 \endcond
 */
