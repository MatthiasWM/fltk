//
// Button header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2014 by Bill Spitzak and others.
//
// This library is free software. Distribution and use rights are outlined in
// the file "COPYING" which should have been included with this file. If this
// file is missing or damaged, see the license at:
//
//     https://www.fltk.org/COPYING.php
//
// Please see the following page on how to report bugs and issues:
//
//     https://www.fltk.org/bugs.php
//

/* \file
   Fl_Button widget . */

#ifndef FL_BUTTON_SHIM_H
#define FL_BUTTON_SHIM_H

#include "Fl_Widget.H"
#include <fltk3/Button.H>

// // values for type()
// #define FL_NORMAL_BUTTON        0   /**< value() will be set to 1 during the press of the button and
//                                          reverts back to 0 when the button is released */
// #define FL_TOGGLE_BUTTON        1   ///< value() toggles between 0 and 1 at every click of the button
// #define FL_RADIO_BUTTON         (FL_RESERVED_TYPE+2) /**< is set to 1 at button press, and all other
//                                          buttons in the same group with <tt>type() == FL_RADIO_BUTTON</tt>
//                                          are set to zero.*/
// #define FL_HIDDEN_BUTTON        3   ///< for Forms compatibility


class Fl_Widget_Tracker;

class FL_EXPORT Fl_Button : public Fl_Widget, public IButtonShim {

  int shortcut_;
  char value_;
  char oldval;
  uchar down_box_;
  uchar compact_;

public:
  Fl_Button(int X, int Y, int W, int H, const char *L = 0)
  : Fl_Widget(new fltk3::Button(this, X, Y, W, H, L)) {}
  ~Fl_Button() override;

  void clear_button() override {
    static_cast<fltk3::Button*>(impl_)->value(0);
  }

protected:

  static void key_release_timeout(void*);
  void simulate_key_action();

  void draw() override {
    static_cast<fltk3::Button*>(impl_)->draw_impl_();
  }

public:

  int handle(int event) override {
    return static_cast<fltk3::Button*>(impl_)->handle_impl_(event);
  }


  int value(int v);
  char value() const;
  int set();
  int clear();
  void setonly();
  int shortcut() const;
  void shortcut(int s);
  Fl_Boxtype down_box() const;
  void down_box(Fl_Boxtype b);
  void shortcut(const char *s);
  Fl_Color down_color() const;
  void down_color(unsigned c);
  void compact(uchar v);
  uchar compact() const;
};

Fl_Button::~Fl_Button() {
  delete static_cast<fltk3::Button*>(impl_);
}

#endif // FL_BUTTON_SHIM_H
