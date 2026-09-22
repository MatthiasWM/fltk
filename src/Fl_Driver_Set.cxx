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

#include "Fl_Driver_Set.H"

#include <stdlib.h>

/**
 \brief Return the active driver set, constructing the built-in one on first call.

 The built-in driver set (the one this platform's fl_*_platform_init.cxx
 provides) is always constructed and registered first. If FLTK_BACKEND is
 set and names a driver set that is registered - for example one loaded at
 runtime via Fl_Plugin_Manager::load(), before this function was first
 called - that one is used instead. Otherwise the built-in driver set is
 used.
*/
Fl_Driver_Set *Fl_Driver_Set::current() {
  static Fl_Driver_Set *default_inst = make();
  const char *want = ::getenv("FLTK_BACKEND");
  if (want) {
    Fl_Plugin_Manager pm(klass());
    Fl_Plugin *p = pm.plugin(want);
    if (p) return (Fl_Driver_Set*)p;
  }
  return default_inst;
}
