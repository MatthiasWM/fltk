
# Rethinking the FLTK Driver system #

## Long Term Goal ##

Have a sound and flexible driver system that allows for ling-time plugins.

I want to be able to compile core FLTK with some of the driver code disabled. For example, I want to be able to compile FLTK without X11 or Cocoa or WIN32 and instead link with an external
Graphics and Window driver set that hooks into the existing Fl_XX_Driver system.

I want to be able to add SDL3 as a backbone for FLTK.
Another developer wants to add his Vulkan driver. I also want to separate the
OpenGL driver into a back compatible OpenGL1 driver and a new OpenGL3 driver
with better integration for better performance.

## Short term goal ###

In particular, I want to add SDL3 support so FLTK can run on Android, iOS, and
Webassembly/Emscripten.

## How to get there ##

These are my ideas on how to get there. Claude may update this file as we find
out better ways to reach the goal.

### Evaluate the current driver code ###

The current code is sometime convoluted, sometimes confuses Driver vs. Context.
We need to find a mostly back compatible way to clean up to code enough to
allow a plugin system for drivers.

### Drivers are plugins ###

We need to be able to register drivers at link time. We need to be able to
link statically and dynamically. FLTK apps should never need to know which
drivers were loaded.

Code in FL/platform.H should be kept at a minimum.

### Build System ###

We need to modify CMake so that we can compile FLTK with just base drivers.
The base drivers do no work, but ensure that FLTK apps still run with minimal
system support and no actual graphics rendering or window building. This is so
an external library can be linked that fills in the missing portions.

### Driver API ###

We need separate documentation for driver developers taht expplains only the driver API
and how to build and link custom drivers.

### Building Drivers ###

We need a minimla example. New drivers can be developed based on this example
as a first scaffolding with quick first gratification.

### Limitations ###

The driver system should stay mostly back compatible with what we have. Since
it is much less exposed than the core FLTK, some incompatible API changes
are allowed if docuemted well.

We stick with C++11 and CMake.

## Roadmap to the short term goal (SDL3) ##

Findings so far: platform selection currently happens entirely at CMake time,
via conditional source-file lists in src/CMakeLists.txt (e.g. FLTK_USE_WAYLAND
appends Wayland .cxx files). There is no runtime/link-time driver registration
yet. There is already a small precedent for a "do-nothing" driver:
src/drivers/Base/Fl_Base_Pen_Driver.H/.cxx.

Steps, in order:

1. **Audit the driver interfaces.** DONE — see DRIVER_AUDIT.md for the full
   inventory (every driver base class, its lifetime, its factory, and where
   Driver/Context state is tangled). Headline findings: selection is 100%
   compile-time via CMake source-file lists (one runtime exception: the
   Wayland build's attempt_wayland() dispatch); a real plugin mechanism
   (Fl_Plugin/Fl_Device_Plugin) already exists and is unused for this;
   driver lifetimes are not uniform (app singleton vs. one-per-window vs.
   one-per-operation); and the clearest Driver/Context leaks are in
   Fl_Graphics_Driver (font/color/matrix/clip state), Fl_Surface_Device
   (global "current surface" pointer), and Fl_Window_Driver (public mutable
   fields + global statics).
2. Find and replace the current selection mechanism: each driver type has a
   factory (e.g. a newScreenDriver()-style singleton chosen by #ifdef).
   Replace the #ifdef-per-platform pattern with link-time self-registration.
   Evaluate reusing Fl_Plugin/Fl_Plugin_Manager (FL/Fl_Device.H) before
   inventing a new registry, and read the Wayland attempt_wayland() pattern
   (src/drivers/Wayland/fl_wayland_platform_init.cxx) as a working precedent
   for runtime backend selection. The registry needs to support both
   singleton-style drivers and per-instance factories (see DRIVER_AUDIT.md). <- WE ARE HERE
3. Extend the "Base" (null) driver to every driver type, not just Pen, so a
   base-only build compiles, links, and runs with no rendering.
4. Add a CMake backend switch (FLTK_BACKEND=X11|WAYLAND|COCOA|WINAPI|SDL3|NONE)
   replacing the implicit OS-based branching.
5. Implement the SDL3 driver set on desktop first (macOS/Linux), using the
   null drivers as scaffolding. Order: Screen -> Window -> Graphics, then
   System, then Gl_Window if needed.
6. Port to Android/iOS once the platform differences live behind the driver
   interface; mostly toolchain/CMake/app-lifecycle work at that point.
7. Write the driver-developer docs and the minimal example driver once SDL3
   has validated the plugin API in practice.


