
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
3. **Extend the "Base" (null) driver to every driver type.** DONE — see
   `src/drivers/Base/`: `Fl_Base_Screen_Driver`, `Fl_Base_System_Driver`,
   `Fl_Base_Window_Driver`, `Fl_Base_Graphics_Driver`,
   `Fl_Base_Copy_Surface_Driver`, `Fl_Base_Image_Surface_Driver`, bundled by
   `Fl_Base_Driver_Set` in `fl_base_platform_init.cxx` (registered as
   `"base"`) — the fifth `Fl_Driver_Set`, same shape as Cocoa/X11/Wayland/
   WinAPI, requiring zero changes to `Fl_Driver_Set` itself. Most of these
   classes only needed to expose a protected constructor; the six driver base
   classes were already nearly instantiable as-is. Two real bugs were found
   and fixed along the way (not just cosmetic — both are latent in the base
   classes today, just never exercised because every real platform's
   override happens to paper over them):
   - `Fl_Screen_Driver`'s constructor sets `num_screens = -1`, and the
     default `init()` is a no-op, so `screen_count()` returned **-1**, not
     the intended fallback of 1. Fixed in `Fl_Base_Screen_Driver`'s
     constructor (`num_screens = 1`).
   - `Fl_System_Driver::wait(double)`'s default body computes the timeout
     bookkeeping and returns the remaining time, but never actually
     blocks/sleeps — reused unmodified, it would busy-spin an event loop at
     100% CPU. `Fl_Base_System_Driver::wait()` calls the inherited
     bookkeeping, then actually sleeps. Verified with `/usr/bin/time`: the
     smoke test below reports ~0s of `user` CPU time despite three 0.1s
     waits, confirming it isn't spinning.

   Verification found three more cross-cutting things that are **not** part
   of `Fl_Driver_Set`'s six factories and have no "base" fallback of their
   own (same category as `Fl_Gl_Window_Driver`/`Fl_Sys_Menu_Bar_Driver`,
   excluded in step 2 for the same reason - separate factory mechanisms):
   `Fl_Native_File_Chooser` (three platform `.cxx`/`.mm` implementations,
   no fourth), `Fl_PDF_File_Surface::new_platform_pdf_surface_` (ditto -
   worked around by also setting `FL_NO_PRINT_SUPPORT` for
   `FLTK_BACKEND_NONE` builds, since print/PDF support is a real optional
   subsystem, not something to stub out), and the global `Window fl_window`
   (every platform's own core file defines this one; `Fl_Base_Driver_Set`'s
   file now does too, for link completeness only - it's not one of the six
   driver factories). Worth remembering for step 4's real `FLTK_BACKEND`
   switch and for whoever eventually designs a driver-plugin API doc.

   CMake: added `FLTK_BACKEND_NONE` (`CMake/options.cmake`, next to the
   existing `FLTK_BACKEND_X11` precedent) and a first `if(FLTK_BACKEND_NONE)`
   branch in `src/CMakeLists.txt`'s `DRIVER_FILES` chain, explicitly
   commented as a verification-only stepping stone toward step 4's real
   switch, not the final design - it doesn't skip Cocoa/X11/GL library
   detection, only which driver sources get compiled in. Also needed:
   excluding the Apple-only `Fl_cocoa.mm`/native-file-chooser/sys-menu-bar/
   pen `.mm` files (these are unconditional on `APPLE`, independent of
   `Fl_Driver_Set` selection, found only by attempting the build).

   Verified: fresh `build/ninja-none` tree (`-DFLTK_BACKEND_NONE=ON`), core
   `fltk` library builds and links clean, and a small smoke test
   (`test/base_smoke.cxx`, gated behind the same option, exercises all six
   factories) builds, links, and exits with `base_smoke: OK` in ~0.6s
   wall-clock with ~0s user CPU time.

   Also proven end-to-end since: an *external*, independently-built shared
   library (`driverset-demo/`, own standalone CMake project, not
   `add_subdirectory()`'d by anything) implementing a driver set out of the
   Base classes above plus the real platform System driver (mix-and-match,
   not all-Base-or-nothing), loaded into a normal FLTK app at runtime via
   `Fl_Plugin_Manager::load()` + `FLTK_BACKEND=demo`, with no changes to
   FLTK itself. This is the concrete validation of the long-term goal's
   "link with an external Graphics and Window driver set". See
   `driverset-demo/README.md` findings below before repeating this: (a) the
   driver API is genuinely private today (`Fl_Driver_Set.H` and the Base
   headers live under `src/`, not `FL/`) - the demo's `CMakeLists.txt`
   reaches into the FLTK source tree directly via a `FLTK_SOURCE_DIR` cache
   variable, there is no public path yet (that's step 7's job); (b) an
   external plugin `.dylib`/`.so` must NOT link FLTK's static library into
   itself - it must resolve FLTK's symbols against the host process at
   dlopen time (`-undefined dynamic_lookup` on macOS), or every FLTK symbol
   (including Fl_cocoa.mm's Objective-C classes) ends up duplicated between
   the plugin and the host executable, which the ObjC runtime correctly
   flags as likely to cause "spurious casting failures and mysterious
   crashes" - worth designing for explicitly whenever step 4/5 makes
   dynamic driver loading a first-class, documented feature rather than a
   demo.
4. Add a CMake backend switch replacing the implicit OS-based branching.
   **Constraint (firm, not up for revisiting):** keep the existing
   per-backend boolean option shape (`FLTK_BACKEND_X11`, `FLTK_BACKEND_NONE`,
   future `FLTK_BACKEND_SDL3`, ...), matching what's already there - not a
   single string-valued `FLTK_BACKEND=X11|WAYLAND|COCOA|WINAPI|SDL3|NONE`
   option. The other FLTK developers have already decided this. <- WE ARE HERE
5. Implement the SDL3 driver set on desktop first (macOS/Linux), using the
   null drivers as scaffolding. Order: Screen -> Window -> Graphics, then
   System, then Gl_Window if needed.
6. Port to Android/iOS once the platform differences live behind the driver
   interface; mostly toolchain/CMake/app-lifecycle work at that point.
7. Write the driver-developer docs and the minimal example driver once SDL3
   has validated the plugin API in practice.


