//
// SDL header file for the Fast Light Tool Kit (FLTK).
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

// Do not directly include this file, instead use <FL/platform.H>.  It will
// include this file if "FLTK_USE_SDL" is defined.  This is to encourage
// portability of even the system-specific code...
#ifndef FL_DOXYGEN

#if !defined(FL_PLATFORM_H)
#  error "Never use <FL/fl_sdl.H> directly; include <FL/platform.H> instead."
#endif // !FL_PLATFORM_H

#include <FL/Fl_Widget.H> // for Fl_Callback

#if (defined(FL_LIBRARY) || defined(FL_INTERNALS)) // this part must be compiled when building the FLTK libraries

// Standard SDL C/C++ includes...
#include <SDL2/SDL.h>

class FLWindow
{
public:
  FLWindow();
};

typedef class FLWindow *Window; // pointer to the FLWindow objective-c class

extern SDL_Renderer *fl_gc;

// TODO: SDL: Region defined here?

#endif // FL_LIBRARY || FL_INTERNALS

//extern CGContextRef fl_gc;

#endif // FL_DOXYGEN
/** \file
 SDL-specific symbols.
 */

/** \defgroup group_sdl SDL-specific symbols
 SDL-specific symbols declared in <FL/platform.H>
 \sa \ref osissues_sdl
 \{ */

///** Returns the macOS-specific graphics context for the current window */
//extern CGContextRef fl_mac_gc();

/** @} */

