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
#include "Fl_SDL_System_Driver.H"
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
  return new Fl_SDL_System_Driver();
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


  Show the Window
 =================

 Windows under FLTK are a bit curious. Their host counterpart is stored in
 Fl_Window_Driver which is not so much of a driver, but a private data and
 function collection. There is a new driver allocated for every top-level
 window. Window drivers have an Fl_X memebr named flx which in turn manages an
 abstract xid which is used to finally link to the host window.

 Under SDL, we have no host windows, just one big screen, so we will implement
 window behaviour in the "driver", and the xid will point back to that. I eneded
 up taking the easy code from teh Cocoa driver and implement show()
 and makeWindow().


  Main Event Loop
 =================

 Next I moved the main event loop into its final location at Fl::wait() which
 is implemented in Fl_System_Driver. This is a bit of an annyance because we
 will have to derive from Posix for most systems, but from the Windows API
 for MSWindow. I am considering to derive ths SDL_System_Driver directly from
 SDL without going through Posix, but that would be quite limiting.

 Anyway, Fl_SDL_System_Driver::wait() gets a rough implementeation so it can
 handle the main event loop and call flush() to update screen graphics.


  Flush
 =======

 Fl::flush() calls the window driver flush() on every window that has a damage
 flag set, and then calls the screen driver flush() in any case. For SDL, we
 have to use the window flushes to mark the area that actually changed and
 then swap the changed areas out to the SDL surface.

 Window flush() calls make_current() which we will use to prep the graphics
 context to have the correct screen offset and clipping area to draw the
 window contents.

 I added a flag and some pointers in the graphics driver that indicate when
 Screen flush() must copy the SDL surface to teh SDL window. So Screen flush()
 now calls SDL_UpdateWindowSurface() whenever we did draw something.


  Graphics Context
 ==================

 Single-stepping through a simple app like "hello", we should now reach
 Fl_Window::draw(). To call any SDL functions via our main graphics driver,
 we need to fill the graphics context (gc) with data like the SDL surface
 pointer, the window origin, and the current clipping area. Let's implement
 Fl_SDL_Window_Driver::make_current() to prepare everything for drawing. TO
 make things easy, the gc is simply a pointer to the graphics driver.

 The first two graphics calls to implement are fl_color() and fl_rectf(). If
 those work, we will see a gray rectangle where the window will be. If the
 window background graphics appear, we are heading into a great direction.

 If this was easy, let's implement all the line drawing functions while we are
 att it. No parameters besides color yet.


  Mouse Events
 ==============

 Now to verify that our minimal event loop is working by implementing mouse
 events. That will allow us to click on buttons, see their graphics change,
 and trigger callbacks.

 Handling mouse clicks revealed what I was afraid of. In the current mode,
 the SDL renderer expects us to redraw the entire screen surface. FLTK is
 very careful to redraw only things that actually changed, which greatly
 reduces reder times.


  Incremental Rendering
 =======================

 I have to find a way to preserve the screen contents between SDL_RenderPresent
 calls. The solution is to render via an external buffer that is copied to the
 renderer at every frame. What sound expensive may not be too bad because
 blitting rectangular pixel arrays around is almost always done in hardware
 these days. SDL provides a texture interface that is optimized to do just that.

 =======================
 === First Milestone ===
 =======================

 We have reached our first milestone here. The CMake setup can compile demo apps
 on MacOS. We open a window that renders something (rectangles) and reacts to
 some event (mouse clicks).

 Next milestone will include the rendering of text, mouse move events, and
 keyboard support.

 I will then adapt teh build system to work on MacOS fo iOS, and compile on
 MSWindows and Linux for that platform.  If everything goes well and I feel
 brave, I will try to get CMake to compile for Android, preferably on all
 three platforms.

 I then need to implement a minimal desktop and window manager. They will
 require complex clipping areas. It would also be nice to OpenGL running in
 FLTK windows and subwindows.

 After that, it's mostly refining event handling and completing all graphics
 calls. Testing, and documentation.


  Rendering Text
 ================

 Adding SDL2_ttf to the CMake system was more complicated than I thought. All
 additional libraries must be exported and added to the export lists. Anyway,
 it took me quite some experimenting, but it works pretty nicely now.

 */
