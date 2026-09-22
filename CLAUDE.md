
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
2. **Replace the per-platform factory functions with `Fl_Driver_Set`.** DONE
   — see src/Fl_Driver_Set.H/.cxx. The six `Fl_Xxx_Driver::newXxxDriver()`
   static factories, previously defined once per platform in
   `fl_*_platform_init.cxx`, are now bundled as pure virtual methods
   (`create_screen_driver()`, `create_system_driver()`,
   `create_window_driver()`, `create_main_graphics_driver()`,
   `create_copy_surface_driver()`, `create_image_surface_driver()`) on one
   abstract `Fl_Driver_Set` class. Each platform's `fl_*_platform_init.cxx`
   now defines exactly one concrete subclass (`Fl_Cocoa_Driver_Set`,
   `Fl_X11_Driver_Set`, `Fl_Wayland_Driver_Set`, `Fl_WinAPI_Driver_Set`) plus
   the single out-of-line body of `Fl_Driver_Set::make()` that constructs it
   — the same link-time-selection shape the old factories had (CMake still
   compiles exactly one `platform_init.cxx` in), just consolidated behind one
   interface instead of six free functions. `Fl_Driver_Set::current()` is the
   one call site everything else goes through; it constructs the driver set
   lazily via a function-local static.

   `Fl_Driver_Set` now derives from `Fl_Plugin` and registers itself (klass
   `"fltk:driverset"`) as a side effect of construction — this is real,
   reusable link-time-and-dynamic-loading infrastructure, not a hand-rolled
   registry, and it's what makes the "priority/override" idea below possible
   later without redesigning anything. Two hazards had to be worked around
   to get there safely (both explained in `Fl_Driver_Set.H`'s doc comment
   and DRIVER_AUDIT.md):
   (a) `Fl_Plugin_Manager` is built on `Fl_Preferences`, whose constructor
   called `Fl::system_driver()` internally (for string duplication, via
   `fl_strdup()`) — circular, since resolving the driver set would need
   `Fl_Preferences`, which needed the driver set already resolved. Fixed by
   giving `Fl_Preferences.cxx` its own internal, dependency-free string-dup
   helper (`fl_prefs_strdup`, in `Fl_Preferences.cxx`) instead of routing
   through the public `fl_strdup()`/`Fl::system_driver()`. `fl_strdup()`
   itself is untouched (out of scope — used throughout the codebase, no
   driver-layer reason to touch it, even though its own system_driver
   indirection is arguably just as unnecessary).
   (b) A self-registering *global* `Fl_Xxx_Driver_Set` instance (the
   "obvious" link-time-registration shape) crashes intermittently because
   `Fl.cxx` has its own namespace-scope globals (`fl_local_shift` etc.)
   whose dynamic initializers call `Fl::system_driver()`, and C++ does not
   guarantee dynamic-init order across translation units. Fixed by keeping
   the built-in driver set's construction lazy — a function-local static
   inside `Fl_Driver_Set::current()` calls `make()` on first use, which is
   well-defined regardless of any other translation unit's init order. A
   *dynamically loaded* driver set (via `Fl_Plugin_Manager::load()`) does
   not have this problem and can self-register eagerly, since
   `dlopen()`/`LoadLibrary()` always runs after `main()` has started.

   `current()` resolution today: the built-in driver set is always
   constructed and registered first; if `FLTK_BACKEND` is set and names a
   *different* registered driver set (e.g. one `load()`-ed at runtime before
   `current()` was first called), that one is used instead. This already
   gives "an externally linked driver set can override the built-in one" —
   Matthias's idea for a numeric priority system (so an override could win
   automatically without the app or user setting `FLTK_BACKEND`) is a real,
   cheap extension point (`Fl_Driver_Set::plugins()`/a `priority()` virtual)
   but deliberately not built now — no concrete need for it yet, and
   `Fl_Plugin_Manager::group()`/`plugin(index)` ordering is explicitly
   documented as unspecified, so "last registered wins" is not a safe
   substitute if that need arises later.

   Verified: builds and runs clean on Cocoa (this machine) and X11 (via
   XQuartz, `-DFLTK_BACKEND_X11=ON`), including the `FLTK_BACKEND` match and
   fallback paths. WinAPI and Wayland were rewritten by the same mechanical
   pattern but are unverified by a local build — needs a Windows/Linux
   reviewer or CI before merging.
3. Extend the "Base" (null) driver to every driver type, not just Pen, so a
   base-only build compiles, links, and runs with no rendering. <- WE ARE HERE
4. Add a CMake backend switch (FLTK_BACKEND=X11|WAYLAND|COCOA|WINAPI|SDL3|NONE)
   replacing the implicit OS-based branching.
5. Implement the SDL3 driver set on desktop first (macOS/Linux), using the
   null drivers as scaffolding. Order: Screen -> Window -> Graphics, then
   System, then Gl_Window if needed.
6. Port to Android/iOS once the platform differences live behind the driver
   interface; mostly toolchain/CMake/app-lifecycle work at that point.
7. Write the driver-developer docs and the minimal example driver once SDL3
   has validated the plugin API in practice.


