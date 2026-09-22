//
// Demonstration external Fl_Driver_Set for FLTK.
//
// Built entirely independently of the main FLTK CMake project (see
// CMakeLists.txt in this directory) and loaded at runtime via
// Fl_Plugin_Manager::load(), i.e. dlopen()/LoadLibrary(). This is the
// proof-of-concept for CLAUDE.md's long-term goal: "compile core FLTK
// with some of the driver code disabled ... and instead link with an
// external Graphics and Window driver set."
//
// Every driver here is the no-op "Base" one from src/drivers/Base/
// (see CLAUDE.md step 3), except the System driver, which is the real
// platform one - this driver set has no rendering/windowing of its own,
// but still has real file I/O, threading, and awake-handler support.
//
// Note the private-header dependency: Fl_Driver_Set.H, the Base driver
// headers, and the real per-platform System driver header all live under
// FLTK's src/, which is not part of its public installed/exported
// interface (see this directory's CMakeLists.txt, FLTK_SOURCE_DIR). The
// driver API isn't public yet - that's a later roadmap step.
//

#include <Fl_Driver_Set.H>
#include <drivers/Base/Fl_Base_Screen_Driver.H>
#include <drivers/Base/Fl_Base_Window_Driver.H>
#include <drivers/Base/Fl_Base_Graphics_Driver.H>
#include <drivers/Base/Fl_Base_Copy_Surface_Driver.H>
#include <drivers/Base/Fl_Base_Image_Surface_Driver.H>

#if defined(__APPLE__)
#  include <drivers/Darwin/Fl_Darwin_System_Driver.H>
typedef Fl_Darwin_System_Driver Fl_Demo_System_Driver;
#elif defined(_WIN32)
#  include <drivers/WinAPI/Fl_WinAPI_System_Driver.H>
typedef Fl_WinAPI_System_Driver Fl_Demo_System_Driver;
#else
#  include <drivers/Unix/Fl_Unix_System_Driver.H>
typedef Fl_Unix_System_Driver Fl_Demo_System_Driver;
#endif


class Fl_Demo_Driver_Set : public Fl_Driver_Set {
public:
  Fl_Demo_Driver_Set() : Fl_Driver_Set("demo") { }

  Fl_Screen_Driver *create_screen_driver() override {
    return new Fl_Base_Screen_Driver();
  }

  // The one driver NOT based on Base: real system integration.
  Fl_System_Driver *create_system_driver() override {
    return new Fl_Demo_System_Driver();
  }

  Fl_Window_Driver *create_window_driver(Fl_Window *w) override {
    return new Fl_Base_Window_Driver(w);
  }

  Fl_Graphics_Driver *create_main_graphics_driver() override {
    return new Fl_Base_Graphics_Driver();
  }

  Fl_Copy_Surface_Driver *create_copy_surface_driver(int w, int h) override {
    return new Fl_Base_Copy_Surface_Driver(w, h);
  }

  Fl_Image_Surface_Driver *create_image_surface_driver(int w, int h, int high_res, Fl_Offscreen off) override {
    return new Fl_Base_Image_Surface_Driver(w, h, high_res, off);
  }
};

// A self-registering global is safe here, unlike for FLTK's own built-in
// driver sets (see Fl_Driver_Set.H's doc comment for why): this file is
// only ever reached via dlopen()/LoadLibrary() through
// Fl_Plugin_Manager::load(), which always runs well after main() has
// started - never racing another translation unit's static initializers.
static Fl_Demo_Driver_Set demo_driver_set;
