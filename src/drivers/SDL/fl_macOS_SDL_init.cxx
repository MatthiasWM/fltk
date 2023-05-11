//
// macOS-specific code to initialize SDL support.
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


#include "Fl_SDL_Copy_Surface_Driver.H"
#include "Fl_SDL_Graphics_Driver.H"
#include "Fl_SDL_Screen_Driver.H"
#include "../Darwin/Fl_Darwin_System_Driver.H"
#include "Fl_SDL_Window_Driver.H"
#include "Fl_SDL_Image_Surface_Driver.H"


Fl_Copy_Surface_Driver *Fl_Copy_Surface_Driver::newCopySurfaceDriver(int w, int h)
{
  return new Fl_SDL_Copy_Surface_Driver(w, h);
}


Fl_Graphics_Driver *Fl_Graphics_Driver::newMainGraphicsDriver()
{
  return new Fl_SDL_Graphics_Driver();
}


Fl_Screen_Driver *Fl_Screen_Driver::newScreenDriver()
{
  return new Fl_SDL_Screen_Driver();
}


Fl_System_Driver *Fl_System_Driver::newSystemDriver()
{
  return new Fl_Posix_System_Driver();
//  return new Fl_Darwin_System_Driver();
}


Fl_Window_Driver *Fl_Window_Driver::newWindowDriver(Fl_Window *w)
{
  return new Fl_SDL_Window_Driver(w);
}


Fl_Image_Surface_Driver *Fl_Image_Surface_Driver::newImageSurfaceDriver(int w, int h, int high_res, Fl_Offscreen off)
{
  return new Fl_SDL_Image_Surface_Driver(w, h, high_res, off);
}

/*

 Porting Blog:

 This text will follow the efforts to port FLTK to used SDL on all platforms as
 a backend.


  Preparing
 ===========

 Created CMake/sdl.cmake that will contain the option to use SDL as a backend,
 followed by code to download and install SDL using FetchContent_Declare. CMake
 should download SDL at configuration time and will buid SDL as soon as our
 project depends on it.

 Setting the option FLTK_USE_SDL in cmake is the basis for the following steps.
 We need to make sure that we define a macro FLTK_USE_SDL that is reachable
 for the compiler itself, so we add that to fl_config.cmake.in.

 In scr/CMakeList.txt, we add a conditional that replaces all the source and
 header files for Cocoa and Quartz with SDL dirver files. We creat thos by
 copying the original files into a new directory, renaming them, and renaming
 the classes inside. It's a good time to update the comments as well. To make
 this compiel and link, we comment out every class member but constructor
 and destructor. Those are replaced with empty functions.

 I kept the Posix driver, but no longer derive the Darwin driver as it is not
 purely a system driver, but includes menu bar code and other UI stuff.

 In a similar fashion, we add Fl_get_key_SDL.cxx and Fl_Font.H to the driver
 directory and comment almost everything out.

 Another nasty file is FL/platform.H which includes another driver dependent
 header file, mac.H, or whatever else. We copy mac.H to platform_sdl.H and
 again comment almost everything out. We set dummy types for fl_cg, FLWindow,
 and Window. Next we write some empty fl_sdl_gc(), fl_sdl_xid(), etc. .


  Compiling
 ===========

 I compiled to code until there were no more missing constructors, variables,
 or functions. Eventually, a test program like 'hello' should compile,
 generating a program that will run and quit without doing anything
 visible at all. No SDL in the code yet, just minimal driver classes.


  Add SDL
 =========

 Fl_Screen_Driver::get_system_colors() is often the first method that is called,
 so I built the SDL virtual method that simply calls fl_open_display(). That in
 turn calls open_display_platform() which is a great test point for the
 compiler and linker setup. I generated the SDL version of open_display_platform
 and put a minimal working SDL code block inside that opens a window and waits
 for the Quit event. Compiling this code revealed include path issues for
 SDL.h and bad linking of SDL2::SDL2. Since SDL is linked to FLTK, it must be
 exported in sdl.cmake so it can be linked in the very end to the demo apps.

 Now if everything compiles and links, run 'hello', and that minimal window
 should appear and wait until the user closes the window. Great!


  The App Life Cycle
 ====================

 The minimal app life cycle in FLTK is quite simple. AFter openeing the display
 as we did above, the user can create one or many windows. Eventually, we reach
 Fl::run() which callas Fl::wait() which in turn handles all events that host
 system may send to us, and forwards them into the widget hierarchy. Those
 event handlers will determine, if the graphics in the windows somehow cahnged
 and set a damage flag.

 At a certain point, the event loop will stop handling incomming events and
 instead tell all windows that have a damage flag set to draw themselves. AFter
 all redrawing is done, we flush the graphics changes to the display.

 So the minimum required functions are: fl_open_display(),
 Fl_Window_Driver::makeWindow(), Fl::wait(), and Fl::flush()
 

  Main Event Loop
 =================

 Next I moved the main event loop into its final location at Fl::wait


 */
