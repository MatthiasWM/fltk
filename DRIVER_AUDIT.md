# FLTK Driver System Audit — Step 1

Scope: every platform-neutral abstract driver base class (not the per-platform
implementations under `src/drivers/<Platform>/`). Goal: understand what's
there today before designing a link-time registration/plugin mechanism
(roadmap step 2, see [CLAUDE.md](CLAUDE.md)).

## Cross-cutting findings

**Selection today is 100% compile-time.** Four files,
`src/drivers/{X11,Wayland,Darwin,WinAPI}/fl_*_platform_init.cxx`, each define
the same handful of `new*Driver()` static factory functions (plain
`return new Fl_X11_Screen_Driver();`-style bodies — no struct, no registry).
`src/CMakeLists.txt` picks exactly one of these files into the link via an
`if/elseif/elseif/else` chain on `FLTK_USE_X11` / `FLTK_USE_WAYLAND` / `APPLE`.
The **Wayland build is the one existing exception**: it links both Wayland and
X11 driver objects and picks between them *at runtime* inside each factory,
via a local `attempt_wayland()` helper (checks `FLTK_BACKEND` env var,
`XDG_RUNTIME_DIR`, then tries `wl_display_connect`). This is the closest thing
in the codebase today to the runtime-selectable backend behavior we want to
generalize — worth reading closely before designing step 2.

**A real plugin/registry mechanism already exists and is now used for driver
sets too — but getting there took two rounds of debugging.**
`FL/Fl_Device.H` defines `Fl_Device_Plugin : public Fl_Plugin`, using the
pre-existing `Fl_Plugin` / `Fl_Plugin_Manager` machinery. First attempt to
reuse it for `Fl_Driver_Set` crashed: `Fl_Plugin_Manager` is built on
`Fl_Preferences`, and `Fl_Preferences::Node`'s constructor calls
`fl_strdup()` unconditionally to duplicate path/name strings — and
`fl_strdup()` (`fl_string_functions.cxx`) is hard-wired to
`Fl::system_driver()->strdup()`. Resolving the driver set would need
`Fl_Preferences`, which needed `fl_strdup()`, which needed the driver set
already resolved. (The MEMORY-only preferences root, used automatically
here via `Fl_Preferences`'s parent-less/`runtimePrefs` constructor path, was
*not* the problem — that part already avoided the locale/filesystem code;
the string duplication was unconditional regardless of root type.) Fixed by
giving `Fl_Preferences.cxx` its own internal `fl_prefs_strdup()` (plain
`::strdup`/`::_strdup`, no driver indirection) instead of the public
`fl_strdup()` — `Fl_System_Driver::strdup()`'s two real implementations were
already just one-line calls to those same functions, so nothing behavioral
changed, and the public `fl_strdup()` (used throughout the rest of the
codebase) was deliberately left untouched, out of scope.

Second pitfall, independent of the first: a self-registering *global*
`Fl_Xxx_Driver_Set` object doesn't work either, even once `Fl_Plugin` itself
is safe to call early — `Fl.cxx` has its own namespace-scope globals
(`fl_local_shift` and friends) whose dynamic initializers call
`Fl::system_driver()`, and C++ doesn't guarantee cross-translation-unit
dynamic-init order, so a global driver-set object living in a different
`.cxx` file isn't guaranteed to register before those run. Fixed by keeping
driver-set construction lazy (construct-on-first-use via a function-local
static in `Fl_Driver_Set::current()`), which sidesteps ordering entirely.
See `src/Fl_Driver_Set.H`'s doc comment for the full reasoning on both.

**Almost nothing is pure virtual.** Across every driver class audited, `= 0`
is rare to absent — nearly every virtual has a default body (usually a no-op,
sometimes a real cross-platform algorithm). Practically: a lot of "extend the
Base driver to every type" (step 3) may already work close to out of the box,
since the base classes are already close to instantiable/no-op by default.
What needs checking is whether a zero-platform-driver build actually compiles
and links (i.e. no core code assumes a *specific platform's* driver exists,
or reaches for platform-only symbols).

**Instantiation lifetime is not uniform** — this shapes what a registration
API needs to support:
- **App-wide singleton:** Screen, System, Sys_Menu_Bar (macOS only, others
  return NULL), Pen (global reference).
- **One per logical object, created on demand:** Window (one per `Fl_Window`),
  Gl_Window (one per `Fl_Gl_Window`), Copy_Surface / Image_Surface (one per
  operation, not cached).

A single "give me driver X" registry call isn't enough — window/surface
drivers need something more like a registered *factory* invoked per instance.

**Driver vs. Context confusion (the specific problem CLAUDE.md calls out),
concrete instances found:**
- `Fl_Graphics_Driver` stores current font/size/color (`font_`, `size_`,
  `color_`), the transform stack (`matrix_stack`, `m`), the clip-region stack
  (`rstack`), and an in-progress-shape state machine (`what`, `xpoint`,
  `p_size`) directly on the driver. All of that is "what's currently being
  drawn," not "how this platform draws."
- `Fl_Surface_Device` keeps a single global "current surface" pointer
  (`surface_`, private static) reached through `Fl_Surface_Device::surface()`
  — ambient context state behind a static accessor.
- `Fl_Window_Driver` mixes legitimate per-window state (`pWindow`,
  `screen_num_`) with raw **public** mutable fields that are really "what's
  being drawn into right now" (`other_xid`, `wait_for_expose_value`, no
  accessor wrapping), plus purely global UI state that has nothing to do with
  any one window (`static is_a_rescale_`, `static current_menu_button`).
- `Fl_System_Driver` hosts a fully generic, platform-independent cross-thread
  "awake handler" ring buffer as static data. That's core FLTK machinery, not
  platform capability — it shouldn't need to move if/when the System driver
  becomes swappable, but ideally it lives somewhere neutral instead.
- Driver classes reach into each other's statics: the System driver singleton
  pointer is actually stored as `Fl_Screen_Driver::system_driver`, not on
  `Fl_System_Driver` itself. Likely not the only cross-wiring of this kind —
  worth grepping for more during step 2.

**Core code already funnels driver *access* through a few choke points** —
`Fl::screen_driver()`, `Fl::system_driver()`, `Fl_Window_Driver::driver(win)`
— even though *construction* is per-platform/compile-time. That narrows the
blast radius for step 2: most core call sites won't need to change, only the
handful of `new*Driver()` factory definitions and their call sites.

## Per-class inventory

| Class | Header | Public? | Lifetime | Factory | Notes |
|---|---|---|---|---|---|
| `Fl_Screen_Driver` | `src/Fl_Screen_Driver.H` (262 ln) | private | app singleton | `newScreenDriver()`, lazy static in `Fl::screen_driver()` (Fl.cxx:144) | Also hosts the `Fl_System_Driver*` singleton pointer (cross-driver coupling), a keymap table, and global color/scale flags mixed with real capability data. |
| `Fl_System_Driver` | `src/Fl_System_Driver.H` (243 ln) | private | app singleton | `newSystemDriver()`, lazy in `Fl::system_driver()` (Fl.cxx:152), pointer stored on `Fl_Screen_Driver` | ~70 virtuals, all with defaults. Hosts the awake-handler ring buffer as static data. |
| `Fl_Window_Driver` | `src/Fl_Window_Driver.H` (222 ln) | private | one per `Fl_Window` | `newWindowDriver(win)`, called from both `Fl_Window` ctors | See context-leak notes above; also has substantial static "menu window" positioning logic shared across all windows. |
| `Fl_Graphics_Driver` | `FL/Fl_Graphics_Driver.H` (380 ln) | **public** | one, reused via `default_driver()` / per-surface | `newMainGraphicsDriver()` | Largest interface (~90 virtuals). Owns the 2D transform stack (`fl_vertex.cxx`) as real shared logic — that part is legitimately platform-neutral and could stay put. |
| `Fl_Scalable_Graphics_Driver` | `src/Fl_Scalable_Graphics_Driver.H` (157 ln) | private | n/a (mixin layer, only X11/Windows derive from it) | none (chosen implicitly by subclassing) | Adds a parallel `*_unscaled()` virtual set; duplicates some drawing-context state (`line_style_`, `line_width_`). |
| `Fl_Gl_Window_Driver` | `src/Fl_Gl_Window_Driver.H` (119 ln) | private | one per `Fl_Gl_Window` | `newGlWindowDriver(win)` — defined per-platform in the GL driver `.cxx`, **not** in the shared `platform_init.cxx` files; only linked when `fltk_gl` is built | Mixes legitimate per-window state (`pWindow`, `current_prog`) with real global GL-context bookkeeping as statics. |
| `Fl_Sys_Menu_Bar_Driver` | `src/Fl_Sys_Menu_Bar_Driver.H` (58 ln) | private | app singleton, **macOS only** | no dedicated factory — reached via `Fl_System_Driver::sys_menu_bar_driver()`, which returns NULL everywhere except Darwin | Defaults all forward to plain `Fl_Menu_Bar` behavior — genuinely a "does nothing extra" base already. |
| `Fl_Copy_Surface_Driver` | `FL/Fl_Copy_Surface.H` (public, shared with `Fl_Copy_Surface`) | **public** | one per copy operation (not cached) | `newCopySurfaceDriver(w,h)`, called from `Fl_Copy_Surface` ctor | Has real pure virtuals (`set_current`, `translate`, `untranslate`) — one of the few classes where the base truly can't be instantiated as-is. |
| `Fl_Image_Surface_Driver` | `FL/Fl_Image_Surface.H` (public, shared with `Fl_Image_Surface`) | **public** | one per image surface (not cached) | `newImageSurfaceDriver(w,h,...)`, called from `Fl_Image_Surface.cxx` (twice) | Also has real pure virtuals (`set_current`, `translate`, `untranslate`, `image()`). |
| `Fl::Pen::Driver` | `src/drivers/Base/Fl_Base_Pen_Driver.H` (118 ln) | private, lives under `drivers/` not `FL/`/bare `src/` | app singleton, global **reference** not pointer | `Fl::Pen::newPenDriver()` | Architecturally different from every other driver: no instance data on the class at all — subsystem state lives as free `extern` globals in `namespace Fl::Pen`. Gated behind `FLTK_HAVE_PEN_SUPPORT`. This is the existing "do-nothing base driver" precedent named in CLAUDE.md. |
| `Fl_Surface_Device` / `Fl_Display_Device` / `Fl_Device_Plugin` | `FL/Fl_Device.H` (129 ln) | **public** | `Fl_Surface_Device` is the root of the *surface* hierarchy (composes, doesn't inherit from, `Fl_Graphics_Driver`) | n/a | Holds the global "current surface" static (`surface_`). `Fl_Device_Plugin` is the existing unrelated plugin mechanism noted above. |

Not found: a class literally named `Fl_Pen_Driver` — the real name is
`Fl::Pen::Driver` in namespace `Fl::Pen`.

## Implications carried into step 2

- A registration mechanism needs at least two shapes: "resolve the one
  app-wide driver of type X" (Screen/System/Sys_Menu_Bar/Pen) and "resolve a
  factory to call per-instance" (Window/Gl_Window/Copy_Surface/Image_Surface).
- `Fl_Plugin`/`Fl_Plugin_Manager` should be evaluated first before designing a
  new registry — it may already do most of what's needed.
- The Wayland `attempt_wayland()` runtime-dispatch pattern is worth reading in
  full (`src/drivers/Wayland/fl_wayland_platform_init.cxx`) as a working
  precedent for "more than one backend linked in, pick one at runtime."
- Context-leak cleanup (font/color/matrix/clip state on `Fl_Graphics_Driver`;
  the current-surface static on `Fl_Surface_Device`; the public mutable
  fields and global statics on `Fl_Window_Driver`) is a separate, sequenceable
  cleanup — it doesn't block step 2's registration work but should happen
  before external/third-party drivers (SDL3, Vulkan) are asked to implement
  these interfaces, or plugin authors will inherit the same confusion.
