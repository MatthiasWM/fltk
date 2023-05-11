//
// Mac header file for the Fast Light Tool Kit (FLTK).
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
// include this file if "__APPLE__" is defined.  This is to encourage
// portability of even the system-specific code...
#ifndef FL_DOXYGEN

#if !defined(FL_PLATFORM_H)
#  error "Never use <FL/mac.H> directly; include <FL/platform.H> instead."
#endif // !FL_PLATFORM_H

//#ifdef __OBJC__
//  @class NSOpenGLContext;
//#  ifndef GL_SILENCE_DEPRECATION
//#    define GL_SILENCE_DEPRECATION 1
//#  endif
//#elif defined(__cplusplus)
//  class NSOpenGLContext;
//#endif /* __OBJC__ */
//extern NSOpenGLContext *fl_mac_glcontext(GLContext rc);
//
//#ifdef __OBJC__
//@class FLWindow; // a subclass of the NSWindow Cocoa class
typedef struct FLWindow {
} FLWindow;
typedef FLWindow *Window;
//#else
//typedef class FLWindow *Window; // pointer to the FLWindow objective-c class
//#endif // __OBJC__

#include <FL/Fl_Widget.H> // for Fl_Callback

#if (defined(FL_LIBRARY) || defined(FL_INTERNALS)) // this part must be compiled when building the FLTK libraries

// Standard MacOS C/C++ includes...
#undef check // because of Fl::check()

extern fl_uintptr_t fl_gc;

#endif

#endif // FL_DOXYGEN
/** \file
 Mac SDL-specific symbols.
 */

/** \defgroup group_sdl SDL-specific symbols
 SDL-specific symbols declared in <FL/platform.H>
 \sa \ref osissues_sdl
 \{ */

/** Returns the macOS-specific graphics context for the current window */
extern fl_uintptr_t fl_sdl_gc();
/** Returns the macOS-specific window reference corresponding to the given Fl_Window object */
extern FLWindow *fl_sdl_xid(const Fl_Window *win);
/** Returns the Fl_Window corresponding to the given macOS-specific window reference */
extern Fl_Window *fl_sdl_find(FLWindow *);
class Fl_Gl_Window;

/** The version number of the running Mac OS X (e.g., 100604 for 10.6.4, 101300 for 10.13).
 FLTK initializes this global variable before main() begins running. If
 the value is needed in a static initializer, a previous call to Fl::system_driver()
 makes sure \ref fl_mac_os_version has been initialized.
 */
extern int fl_sdl_version;

/** @} */

