//
// Windows system header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2018 by Bill Spitzak and others.
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
// include this file if _WIN32 is defined.  This is to encourage
// portability of even the system-specific code...

#ifndef FL_DOXYGEN
#ifndef FL_PLATFORM_H
#  error "Never use <FL/SDL.H> directly; include <FL/platform.H> instead."
#endif // !FL_PLATFORM_H

#include <SDL2/SDL.h>

typedef void* Window; // TODO: use FLTK internal Window emulation

extern FL_EXPORT SDL_Window *fl_display;
extern FL_EXPORT SDL_Renderer *fl_gc;

#endif // FL_DOXYGEN
