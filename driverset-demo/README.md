# External driver-set demo

Proves that an *external*, independently-built library can implement an
FLTK `Fl_Driver_Set` and be loaded into a normal FLTK application at
runtime — the concrete validation of the long-term goal in
[CLAUDE.md](../CLAUDE.md): "compile core FLTK with some of the driver code
disabled ... and instead link with an external Graphics and Window driver
set that hooks into the existing Fl_XX_Driver system."

This directory is its own standalone CMake project. It is **not**
referenced by any `add_subdirectory()` in the main FLTK build — building
FLTK does not build this, and building this does not touch FLTK's own
build tree.

## What's here

- **`Fl_Demo_Driver_Set.cxx`** — a driver set built from the "Base" (null)
  drivers introduced in CLAUDE.md step 3 (`src/drivers/Base/`) for
  Screen/Window/Graphics/Copy_Surface/Image_Surface, **except** the System
  driver, which is the real platform one (`Fl_Darwin_System_Driver` on
  macOS). Demonstrates that a driver set can freely mix Base and real
  components — this one has no rendering/windowing at all, but has real
  file I/O, threading, and awake-handler support. Built as a **shared**
  library, since it's meant to be `dlopen()`'d.
- **`host_demo.cxx`** — a small app that links a normal FLTK build (with
  its own built-in driver set), then loads and selects the demo driver set
  above via `Fl_Plugin_Manager::load()` + the `FLTK_BACKEND` env var, with
  no changes to FLTK itself. Proves the real System driver is active by
  calling `Fl::system_driver()->getcwd()` (Base's default returns `NULL`;
  a real platform driver returns the actual working directory).

## Two things this surfaced worth knowing before doing this again

1. **The driver API is private, not public, today.** `Fl_Driver_Set.H` and
   the Base driver headers live under FLTK's `src/`, not the installed/
   public `FL/` headers. This demo's `CMakeLists.txt` reaches into the
   FLTK *source* tree directly via a `FLTK_SOURCE_DIR` cache variable —
   there's no public API path yet. That's CLAUDE.md step 7's job
   ("separate documentation for driver developers... only the driver API").

2. **A plugin must not link FLTK's static library into itself.** This
   FLTK build produces `libfltk.a` (static). If the plugin `.dylib` links
   it too, every FLTK symbol — including the Objective-C classes from
   `Fl_cocoa.mm` — ends up duplicated between the plugin and whatever host
   executable also links `libfltk.a`, which the Objective-C runtime
   correctly flags at load time ("Class FLWindow is implemented in both
   ... This may cause spurious casting failures and mysterious crashes.").
   The fix: the plugin only links FLTK's *headers*, and resolves FLTK's
   symbols against the host process at `dlopen()` time instead
   (`-undefined dynamic_lookup` on macOS; on Linux/ELF this happens by
   default with no extra flag needed, as long as the host executable was
   linked normally). Worth designing for explicitly once dynamic driver
   loading becomes a real, documented feature rather than a demo.

## Build and run

```sh
cmake -B build -S . -GNinja \
  -DFLTK_DIR=<path to an FLTK build tree, e.g. ../build/ninja> \
  -DFLTK_SOURCE_DIR=<path to this FLTK source checkout>
cmake --build build
cd build && ./host_demo
```

Expected output:
```
system_driver()->getcwd() = /path/to/driverset-demo/build  (real system driver confirmed)
host_demo: OK (real system driver + Base screen/window/graphics/surfaces)
```

If `Fl_Plugin_Manager::load()` were removed from `host_demo.cxx`, everything
after it would run against the build's own built-in driver set instead
(e.g. Cocoa) — that call, plus setting `FLTK_BACKEND=demo` before it, is the
only thing that selects the external one.
