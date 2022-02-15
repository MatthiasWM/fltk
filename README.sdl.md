
Building FLTK with SDL
======================

This option will work on all platforms (Windows, macOS, Linux, Android, iOS)
and replaces all system calls with their corresponding SDL call, making
this a one-source-only solution on all platforms including mobile.

All rendering will take place in a single SDL window. FLTK apps will usually
run in fullscreen mode. However, FLTK will simulate a simple desktop, allowing
for dialog boxes and movable and resizable app windows.

Building SDL
------------

The following text is written as we go.

The first implementation is for macOS where SDL simply replaces all BSD, Cocoa,
etc, calls. First priority is a clean CMake system. The rest will fall into place
as we go.

Since we are not (yet) cross compiling, SDL can be installed as a developer library
on macOS for easy access.

Clone SDL from its GitHub repo. We should agree on a Tag that should be pulled.

For now, we leave SDL at the default settings and call

cmake -S . -B build
cmake --build build
build/test/testdraw2

... and a fickering window should quickly draw rectangles and lines everywhere.

Installing does not work in my current commit:
(66ee79bd68335b9725fd94defc3e5ac5ff3c4249)
so we disable SDL_TEST and build the library once more and install it:

sudo cmake --build build --target install

This gives us the includes at `/usr/local/include/` and the libraries at
`/usr/local/lib/libSDL2.a` and `/usr/local/lib/libSDL2main.a`.

NOTE to FLTK maintainers: SDL adds lots of CMake files in 
`/usr/local/lib/cmake/SDL2/` which help to include SDL in other projects.
Maybe FLTK can take some inspiration here!


Modifying FLTK
--------------

To make this transition as clean as possible (because nobody likes to come
back to old code to fix an outdated hack), we update the CMake system first
to automate linking to SDL. A new option `OPTION_USE_SDL` is added that will
work on all desktop platforms and shall work in combination with 
`FLTK_FOR_ANDROID` and `FLTK_FOR_IOS` to cross-compile for those platforms.

We already have `OPTION_APPLE_X11`, so this is pretty similar. Is this option
still supported and tested btw.?

The pattern seems to be `if (APPLE AND OPTION_APPLE_X11) ...`. So let's
see what we can do with that.

Introduction of `OPTION_USE_SDL`. 

In `setup.cmake`, change the frameworks and libraries to link with.
In `resources.cmake` find SDL2 and set LIB_SDL2 (points to the Framework!)
In `options.cmake`, we add `OPTION_USE_SDL`.
In `variables.cmake`, we seem to set frameworks again
Don't build `Fluid` in `CMakeLists.txt`.
In `src/CMakeLists.txt`, we add driver files for SDL.

Next, we generate all the drivers files - empty for now. So building our
CMake or Makefile generators will be succesful 
(`cmake -S . -B build/Xcode -D OPTION_USE_SDL=YES`).

Compilation will fail at the linking stage with missing sybols, for example
`Fl_Screen_Driver::newScreenDriver()`. So it's time to implement stub drivers
for all the driver file we created earlier. No need to override anything
yet, just inherit from the default drivers.

After adapting all missing functions from another driver, we can add SDL
function calls to initialise SDL and open the main screen. This is also
the perfect occasion to check include and link paths, and verify that we
link everything correctly.

Adding the library in `resources` did not work as expected. New code in 
`options` works for including file. This works for me, but is it the correct
way to do this.

We add the SDL library ${SDL2_LIBRARIES} in `CMake/fl_create_example.cmake`
as a link library, and hooray, everything compiles, links, and run. `SDL_Init()`
is found and called. Of course it's not plced correctly yet, but we have a 
good-enough starting point.

There is a lot missing in the build systems of course: we need to add the correct
settings to the `fltk-config` script, maybe even add a command line option. We
need to make this setup run on all other platforms as well, and we need to 
implement and test the new platforms. After that, we must document things and 
remove debugging text.

But for now, I want to see SDL opening a new screen (on the desktop, SDL opens
a window on the desktop. since SDL is basically a library for games, we assume
that the SDL "window" is actually the desktop screen, as it will be in Android
and iOS anyway. So teh terminaology for SDL window is FLTK screen. FLTK windows
are implemented by this driver).

So we need to place `SDL_Init`, `SDL_CreateWindow`, `SDL_CreateRenderer`, 
`SDL_WaitEventTimeout`, `SDL_DestroyWindow`, and `SDL_Quit` to get a minimal FLTK 
app to run.

Let's check the life cycle of our drivers by adding `printf` statements:
```
SDL System Driver created
SDL Graphics Driver created
SDL Window Driver created
SDL Screen Driver created
SDL Window Driver destroyed
```
Uh huh! So the window driver is the only one that ever gets destroyed. While all
other drivers are allocated once and then never touched again, FLTK will allocate
a new window driver for every new window. So window drivers are not really 
'drivers' in the sense of a factory, but more of a manager. All other drivers are
simply free'd when the app exits without really rooling back anything. If it
turns out that calling `SDL_Quit` is essential, we will have to set some
cross-paltform `atexit()` function. For now, we'll just put it in 
`~Fl_SDL_System_Driver()` for optics and as a counterpoint to `SDL_Init` which 
ssems to fit into `Fl_SDL_System_Driver()`.

Next, we need to create the DSL Window / FLTK Screen. I want to delay the 
creation of the screen until we actualy need it. This gives users a chance
to set strategies about opening the screen (desired size and resoltion i.e.,
but more about that later) before actually opening it. It alos gives
apps a chance to run without creating an windows at all, which we do in
Fluid if it only used as part of a script. So how about placing
`SDL_Create_Window` in `fl_open_display()`? The call actually end in the 
platform dependent call `open_display_platform()` of the corresponding
Screen driver. The counterpart is `close_display()`.

Remembering the original implementation for X11, the SDL Window and Renderer
seem to correspond with the X11 display, or `fl_display`. If we were to use
multiple SDL windows, the renderer would correspond to `fl_xid`, but we
are emuoating a window manager, so we need to emulate `fl_xid` as well as it
contains additional informations like origins and clipping regions.

To make sure that `fl_open_display` is called at all, we need to implement
at least one screen related method, and `get_system_colors()` is the best
candidate.

`SDL_WaitEvent` is next on our list. Tracing `Fl::run()`, we see that not 
having a platform representation of na FLTK window yet will break the event 
loop (checks `Fl_X::first`). So let's implement the SDL version of `Fl_X`
first. It's in `FL/platform.H` which we modify to load `FL/SDL.H`.

Ultimately, we need to implement the `show()` method of the Window driver.
So let's do the minimal thing here and link a new "Window" into `Fl_X::first`.

Great. Now that `Fl::run()` sees our window, we get forwarded all the way to
`wait()` in the System driver. We add our last required SDL call here:
`SDL_WaitEventTimeout()`.

Wow, this is a huge milestone: the screen opens, a window is created, and 
the event loop is running. 

Our very basic structure of everything is implemented. For easier testing,
we implement handling of the 'Escape' key first, so we can easily close our
app while debugging. 

NOTE: first of all, we now have two windows that open! Also, the Escape key is handled, but not closing the first window, so we are not closing the app either.










