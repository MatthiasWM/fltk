//
// Windows-specific code for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2022 by Bill Spitzak and others.
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

// This file contains Windows-specific code for FLTK which is always linked
// in.  Search other files for "_WIN32" or filenames ending in _win32.cxx
// for other system-specific code.

#if !defined(FL_DOXYGEN)

#include <config.h>
#include <FL/Fl.H>
#include <FL/platform.H>
#include "Fl_Window_Driver.H"
#include "Fl_Screen_Driver.H"
#include <FL/Fl_Graphics_Driver.H> // for fl_graphics_driver
#include "drivers/SDL/Fl_SDL_Window_Driver.H"

Window fl_window = NULL;

#endif
