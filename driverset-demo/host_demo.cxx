//
// Host app for the external driver-set demo.
//
// Links a normal FLTK build (with its own built-in driver set - Cocoa,
// X11, whichever one the FLTK tree used to build fltk::fltk was
// configured with) and, before any driver gets resolved, loads and
// selects the demo driver set built in this same directory
// (Fl_Demo_Driver_Set.cxx) instead - entirely via the public
// Fl_Plugin_Manager API and the FLTK_BACKEND env var, no source changes
// to FLTK itself.
//
// If the Fl_Plugin_Manager::load() call below were removed, everything
// after it would run against the build's own built-in driver set instead
// (e.g. Cocoa) - that's the only thing selecting "demo" changes.
//

#include <FL/Fl.H>
#include <FL/Fl_Plugin.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Copy_Surface.H>
#include <FL/Fl_Image_Surface.H>
#include <FL/Fl_RGB_Image.H>
#include <Fl_System_Driver.H> // private header - see this directory's CMakeLists.txt

#include <stdio.h>
#include <stdlib.h>

static void set_backend_env(const char *name) {
#ifdef _WIN32
  _putenv_s("FLTK_BACKEND", name);
#else
  setenv("FLTK_BACKEND", name, 1);
#endif
}

int main() {
  // Must happen before the first call into Fl_Driver_Set::current()
  // (i.e. before constructing any Fl_Window or touching Fl::screen_driver()
  // / Fl::system_driver()).
  set_backend_env("demo");
  if (Fl_Plugin_Manager::load("./libfltk_demo_driverset.dylib") != 0) {
    fprintf(stderr, "FAIL: could not load the demo driver set (expected next to this executable)\n");
    return 1;
  }

  // Fl_Window_Driver, and indirectly Fl_Screen_Driver via Fl::screen_driver()
  Fl_Window *window = new Fl_Window(300, 200);
  window->show();

  // The one non-Base driver: prove the REAL system driver is active.
  // Fl_System_Driver::getcwd()'s default (what every Base-only driver set
  // would return) is NULL; the real platform driver returns the actual cwd.
  char cwd[1024];
  char *got = Fl::system_driver()->getcwd(cwd, sizeof(cwd));
  if (!got) {
    fprintf(stderr, "FAIL: system_driver()->getcwd() returned NULL - looks like Base, not the real system driver\n");
    return 1;
  }
  printf("system_driver()->getcwd() = %s  (real system driver confirmed)\n", got);

  // Fl_Copy_Surface_Driver / Fl_Image_Surface_Driver, same checks as
  // test/base_smoke.cxx.
  Fl_Copy_Surface *cs = new Fl_Copy_Surface(50, 50);
  delete cs;

  Fl_Image_Surface *is = new Fl_Image_Surface(32, 24);
  Fl_RGB_Image *img = is->image();
  if (!img || img->w() != 32 || img->h() != 24) {
    fprintf(stderr, "FAIL: image surface\n");
    return 1;
  }
  delete img;
  delete is;
  delete window;

  printf("host_demo: OK (real system driver + Base screen/window/graphics/surfaces)\n");
  return 0;
}
