//
// Definition of SDL window driver
// for the Fast Light Tool Kit (FLTK).
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

/**
 \file Fl_SD_Window_Driver.H
 \brief Definition of SDL window driver.
 */

#ifndef FL_SDL_WINDOW_DRIVER_H
#define FL_SDL_WINDOW_DRIVER_H

#include "../../Fl_Window_Driver.H"
#include <FL/Fl_Plugin.H>

/**
 \cond DriverDev
 \addtogroup DriverDeveloper
 \{
 */


/**
 \}
 \endcond
 */


class Fl_SDL_Window_Driver : public Fl_Window_Driver
{
//private:
//  struct shape_data_type {
//    Fl_Image* shape_; ///<  shape image
//    CGImageRef mask;
//  } *shape_data_;
//  void shape_bitmap_(Fl_Image* b);
//  void shape_alpha_(Fl_Image* img, int offset) FL_OVERRIDE;
//  CGRect* subRect_;           // makes sure subwindow remains inside its parent window
//  // stores 3 binary flags: whether window is mapped to retina display; whether resolution just changed;
//  // whether window's view received the [FLView view_did_resize] message
//  unsigned window_flags_;
public:
  Fl_SDL_Window_Driver(Fl_Window *w);
  ~Fl_SDL_Window_Driver();
//  static inline Fl_Cocoa_Window_Driver* driver(const Fl_Window *w) {return (Fl_Cocoa_Window_Driver*)Fl_Window_Driver::driver(w);}
//  CGContextRef gc;                 // graphics context
//  NSCursor *cursor;
//  static void q_release_context(Fl_Cocoa_Window_Driver *x = 0); // free all resources associated with gc
//  static void clip_to_rounded_corners(CGContextRef gc, int w, int h);
//  void set_key_window();
//  bool mapped_to_retina();      // is window mapped to retina display?
//  void mapped_to_retina(bool);  // sets whether window is mapped to retina display
//  bool changed_resolution();      // did window just moved to display with another resolution?
//  void changed_resolution(bool);// sets whether window just moved to display with another resolution
//  bool view_resized();   // did window's view receive [FLView view_did_resize] message?
//  void view_resized(bool b); // sets whether window's view received [FLView view_did_resize] message
//  bool through_resize();   // did Fl_Window::resize() run already
//  void through_resize(bool b); // set whether Fl_Window::resize() run already
//  CGRect* subRect() { return subRect_; } // getter
//  void subRect(CGRect *r) { subRect_ = r; } // setter
//  static void destroy(FLWindow*);
//  CGImageRef CGImage_from_window_rect(int x, int y, int w, int h, bool capture_subwins = true);
//
//  // --- window data
//  int decorated_w() FL_OVERRIDE;
//  int decorated_h() FL_OVERRIDE;
//  const Fl_Image* shape() FL_OVERRIDE;
//
//  // --- window management
  void makeWindow() FL_OVERRIDE;
//  void take_focus() FL_OVERRIDE;
//  void flush() FL_OVERRIDE;
//  void flush_overlay() FL_OVERRIDE;
//  void draw_begin() FL_OVERRIDE;
//  void draw_end() FL_OVERRIDE;
  void make_current() FL_OVERRIDE;
//  void label(const char *name, const char *mininame) FL_OVERRIDE;
  void show() FL_OVERRIDE;
//  void resize(int X,int Y,int W,int H) FL_OVERRIDE;
  void hide() FL_OVERRIDE;
//  void map() FL_OVERRIDE;
//  void unmap() FL_OVERRIDE;
//  void fullscreen_on() FL_OVERRIDE;
//  void fullscreen_off(int X, int Y, int W, int H) FL_OVERRIDE;
//  void use_border() FL_OVERRIDE;
//  void size_range() FL_OVERRIDE;
//  void iconize() FL_OVERRIDE;
//  void decoration_sizes(int *top, int *left,  int *right, int *bottom) FL_OVERRIDE;
//  // --- window cursor stuff
//  int set_cursor(Fl_Cursor) FL_OVERRIDE;
//  int set_cursor(const Fl_RGB_Image*, int, int) FL_OVERRIDE;
//
//  void shape(const Fl_Image* img) FL_OVERRIDE;
//  // next 4 are in Fl_cocoa.mm because they use Objective-c
//  void capture_titlebar_and_borders(Fl_RGB_Image*& top, Fl_RGB_Image*& left, Fl_RGB_Image*& bottom, Fl_RGB_Image*& right) FL_OVERRIDE;
//  void wait_for_expose() FL_OVERRIDE;
//  void draw_titlebar_to_context(CGContextRef gc, int w, int h);
//  int scroll(int src_x, int src_y, int src_w, int src_h, int dest_x, int dest_y, void (*draw_area)(void*, int,int,int,int), void* data) FL_OVERRIDE;
//
//  //icons
//  void icons(const Fl_RGB_Image *icons[], int count) FL_OVERRIDE;
//  NSImage *icon_image;
//
//  fl_uintptr_t os_id() FL_OVERRIDE;
};

#endif // FL_SDL_WINDOW_DRIVER_H
