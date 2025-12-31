//
// Widget header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2025 by Bill Spitzak and others.
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
  \file FL/Fl_Widget.H
  \brief Fl_Widget and Fl_Label classes.
*/

#ifndef FL_WIDGET_SHIM_H
#define FL_WIDGET_SHIM_H

#include <fltk3/Fl.H>
#include <fltk3/Widget.H>
// #include "Fl.H"

class Fl_Widget;
class Fl_Window;
class Fl_Group;
class Fl_Image;

// struct FL_EXPORT Fl_Label {
//   /** label text */
//   const char* value;
//   /** optional image for an active label */
//   Fl_Image* image;
//   /** optional image for a deactivated label */
//   Fl_Image* deimage;
//   /** label font used in text */
//   Fl_Font font;
//   /** size of label font */
//   Fl_Fontsize size;
//   /** text color */
//   Fl_Color color;
//   /** alignment of label */
//   Fl_Align align_;
//   /** type of label. \see Fl_Labeltype */
//   uchar type;
//   /** Spacing between label and the horizontally aligned side of the widget. */
//   signed char h_margin_;
//   /** Spacing between label and the vertically aligned side of the widget. */
//   signed char v_margin_;
//   /** Spacing between an image and the label text */
//   uchar spacing;

//   /** Draws the label aligned to the given box */
//   void draw(int,int,int,int, Fl_Align) const ;
//   void measure(int &w, int &h) const ;
// };


// class FL_EXPORT Fl_Callback_User_Data {
// protected:
//   Fl_Callback_User_Data() {} ///< Protected constructor
// public:
//   virtual ~Fl_Callback_User_Data() { } ///< Destructor
// };


class FL_EXPORT Fl_Widget : public IWidgetShim {
  friend class Fl_Group;

public:
  virtual ~Fl_Widget() = default;

private:

  /** unimplemented copy ctor */
  Fl_Widget(const Fl_Widget &);
  /** unimplemented assignment operator */
  Fl_Widget& operator=(const Fl_Widget &);

protected:
  void* impl_ = nullptr;

  //Fl_Widget(int x, int y, int w, int h, const char *label=0L) { }
  Fl_Widget(void *impl)
  : impl_(impl) { }


  void x(int v);
  void y(int v);
  void w(int v);
  void h(int v);
  unsigned int flags() const;
  void set_flag(unsigned int c);
  void clear_flag(unsigned int c);
  enum {
        INACTIVE        = 1<<0,   ///< the widget can't receive focus, and is disabled but potentially visible
        INVISIBLE       = 1<<1,   ///< the widget is not drawn, but can receive a few special events
        OUTPUT          = 1<<2,   ///< for output only
        NOBORDER        = 1<<3,   ///< don't draw a decoration (Fl_Window)
        FORCE_POSITION  = 1<<4,   ///< don't let the window manager position the window (Fl_Window)
        NON_MODAL       = 1<<5,   ///< this is a hovering toolbar window (Fl_Window)
        SHORTCUT_LABEL  = 1<<6,   ///< the label contains a shortcut we need to draw
        CHANGED         = 1<<7,   ///< the widget value changed
        OVERRIDE        = 1<<8,   ///< position window on top (Fl_Window)
        VISIBLE_FOCUS   = 1<<9,   ///< accepts keyboard focus navigation if the widget can have the focus
        COPIED_LABEL    = 1<<10,  ///< the widget label is internally copied, its destruction is handled by the widget
        CLIP_CHILDREN   = 1<<11,  ///< all drawing within this widget will be clipped (Fl_Group)
        MENU_WINDOW     = 1<<12,  ///< a temporary popup window, dismissed by clicking outside (Fl_Window)
        TOOLTIP_WINDOW  = 1<<13,  ///< a temporary popup, transparent to events, and dismissed easily (Fl_Window)
        MODAL           = 1<<14,  ///< a window blocking input to all other windows (Fl_Window)
        NO_OVERLAY      = 1<<15,  ///< window not using a hardware overlay plane (Fl_Menu_Window)
        GROUP_RELATIVE  = 1<<16,  ///< Reserved, not implemented. DO NOT USE.
        COPIED_TOOLTIP  = 1<<17,  ///< the widget tooltip is internally copied, its destruction is handled by the widget
        FULLSCREEN      = 1<<18,  ///< a fullscreen window (Fl_Window)
        MAC_USE_ACCENTS_MENU = 1<<19, ///< On the macOS platform, pressing and holding a key on the keyboard opens an accented-character menu window (Fl_Input_, Fl_Text_Editor)
        NEEDS_KEYBOARD  = 1<<20,  ///< set on touch screen devices if a widget needs a keyboard when it gets the focus. Reserved, not yet used in 1.4.0. \see Fl_Widget::needs_keyboard()
        IMAGE_BOUND     = 1<<21,  ///< binding the image to the widget will transfer ownership, so that the widget will delete the image when it is no longer needed
        DEIMAGE_BOUND   = 1<<22,  ///< bind the inactive image to the widget, so the widget deletes the image when it is no longer needed
        AUTO_DELETE_USER_DATA = 1<<23, ///< automatically call `delete` on the user_data pointer when destroying this widget; if set, user_data must point to a class derived from the class Fl_Callback_User_Data
        MAXIMIZED       = 1<<24,  ///< a maximized Fl_Window
        POPUP           = 1<<25,  ///< popup window (i.e., positioned relatively to another mapped window)
        // Note to devs: add new FLTK core flags above this line (up to 1<<28).

        // Three more flags, reserved for user code

        USERFLAG3       = 1<<29,  ///< reserved for 3rd party extensions
        USERFLAG2       = 1<<30,  ///< reserved for 3rd party extensions
        USERFLAG1       = 1<<31   ///< reserved for 3rd party extensions
  };
  void draw_box() const;
  void draw_box(Fl_Boxtype t, Fl_Color c) const;
  void draw_box(Fl_Boxtype t, int x,int y,int w,int h, Fl_Color c) const;
  void draw_backdrop() const;
  void draw_focus() const;
  void draw_focus(Fl_Boxtype t, int X, int Y, int W, int H) const;
  void draw_focus(Fl_Boxtype t, int x, int y, int w, int h, Fl_Color bg) const;
  void draw_label() const;
  void draw_label(int, int, int, int) const;

public:

  void draw() override { }
  int handle(int event) override { return 0; }
  int is_label_copied() const;
  void needs_keyboard(bool needs);
  bool needs_keyboard() const;
  Fl_Group* parent() const;
  void parent(Fl_Group* p);
  uchar type() const;
  void type(uchar t);
  int x() const;
  int y() const;
  int w() const;
  int h() const;

  virtual void resize(int x, int y, int w, int h) {
    static_cast<fltk3::Widget*>(impl_)->resize(x, y, w, h);
  }

  int damage_resize(int,int,int,int);
  void position(int X,int Y);
  void size(int W,int H);
  Fl_Align align() const;
  void align(Fl_Align alignment);
  Fl_Boxtype box() const;
  void box(Fl_Boxtype new_box);
  Fl_Color color() const;
  void color(Fl_Color bg);
  Fl_Color selection_color() const;
  void selection_color(Fl_Color a);
  void color(Fl_Color bg, Fl_Color sel);
  const char* label() const;
  void label(const char* text);
  void copy_label(const char *new_label);
  void label(Fl_Labeltype a, const char* b);
  Fl_Labeltype labeltype() const;
  void labeltype(Fl_Labeltype a);
  Fl_Color labelcolor() const;
  void labelcolor(Fl_Color c);
  Fl_Font labelfont() const;
  void labelfont(Fl_Font f);
  Fl_Fontsize labelsize() const;
  void labelsize(Fl_Fontsize pix);
  Fl_Image* image();
  const Fl_Image* image() const;
  void image(Fl_Image* img);
  void image(Fl_Image& img);
  void bind_image(Fl_Image* img);
  void bind_image(int f);
  int image_bound() const;
  Fl_Image* deimage();
  const Fl_Image* deimage() const;
  void deimage(Fl_Image* img);
  void deimage(Fl_Image& img);
  void bind_deimage(Fl_Image* img);
  int deimage_bound() const;
  void bind_deimage(int f);
  void label_image_spacing(int gap);
  int label_image_spacing();
  void horizontal_label_margin(int px);
  int horizontal_label_margin();
  void vertical_label_margin(int px);
  int vertical_label_margin();
  const char *tooltip() const;
  void tooltip(const char *text);               // see Fl_Tooltip
  void copy_tooltip(const char *text);          // see Fl_Tooltip
  Fl_Callback_p callback() const;

  void callback(Fl_Callback* cb, void* p) {
    static_cast<fltk3::Widget*>(impl_)->callback(cb, p);
  }

  void callback(Fl_Callback* cb, Fl_Callback_User_Data* p, bool auto_free);
  void callback(Fl_Callback* cb);
  void callback(Fl_Callback0* cb);
  void callback(Fl_Callback1* cb, long p = 0);
  void* user_data() const;
  void user_data(void* v);
  void user_data(Fl_Callback_User_Data* v, bool auto_free);
  long argument() const;
  void argument(long v);
  Fl_When when() const;
  void when(uchar i);
  unsigned int visible() const;
  int visible_r() const;

  virtual void show() {
    static_cast<fltk3::Widget*>(impl_)->show();
  }

  virtual void hide() {
    static_cast<fltk3::Widget*>(impl_)->hide();
  }

  void set_visible();
  void clear_visible();
  unsigned int active() const;
  int active_r() const;
  void activate();
  void deactivate();
  unsigned int output() const;
  void set_output();
  void clear_output();
  unsigned int takesevents() const;
  unsigned int changed() const;
  void set_changed();
  void clear_changed();
  void clear_active();
  void set_active();
  int take_focus();
  void set_visible_focus();
  void clear_visible_focus();
  void visible_focus(int v);
  unsigned int visible_focus() const;
  static void default_callback(Fl_Widget *widget, void *data);
  void do_callback(Fl_Callback_Reason reason=FL_REASON_UNKNOWN);
  void do_callback(Fl_Widget *widget, long arg, Fl_Callback_Reason reason=FL_REASON_UNKNOWN);
  void do_callback(Fl_Widget *widget, void *arg = 0, Fl_Callback_Reason reason=FL_REASON_UNKNOWN);
  int test_shortcut();
  static unsigned int label_shortcut(const char *t);
  static int test_shortcut(const char*, const bool require_alt = false);
  void _set_fullscreen();
  void _clear_fullscreen();
  int contains(const Fl_Widget *w) const;
  int inside(const Fl_Widget *wgt) const;
  void redraw();
  void redraw_label();
  uchar damage() const;
  void clear_damage(uchar c = 0);
  void damage(uchar c);
  void damage(uchar c, int x, int y, int w, int h);
  void draw_label(int, int, int, int, Fl_Align) const;
  void measure_label(int& ww, int& hh) const;
  Fl_Window* window() const;
  Fl_Window* top_window() const;
  Fl_Window* top_window_offset(int& xoff, int& yoff) const;
  virtual Fl_Group* as_group() { return nullptr; }
  virtual Fl_Group const* as_group() const { return nullptr; }
  virtual Fl_Window* as_window() { return nullptr; }
  virtual Fl_Window const* as_window() const { return nullptr; }
  virtual class Fl_Gl_Window* as_gl_window() { return nullptr; }
  virtual class Fl_Gl_Window const* as_gl_window() const { return nullptr; }
  int use_accents_menu();
  void shortcut_label(int value);
  int shortcut_label() const;
};

/**
    Reserved type numbers (necessary for my cheapo RTTI) start here.
    Grep the header files for "RESERVED_TYPE" to find the next available
    number.
*/
// #define FL_RESERVED_TYPE 100

#endif
