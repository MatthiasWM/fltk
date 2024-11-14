//
// Coordinate Input Widget for Fast Light User Interface Designer (FLUID).
//
// Copyright 1998-2024 by Bill Spitzak and others.
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

#ifndef FLUID_WIDGETS_COORD_INPUT_H
#define FLUID_WIDGETS_COORD_INPUT_H

#include <FL/Fl_Input.H>

namespace fluid {

namespace widget {

// Callback signature for function returning the value of a variable.
typedef int (CoordCallback)(class CoordInput const *, void*);

// Entry for a list of variables available to an input field.
// CoordInput::variables() expects an array of CoordInputVars
// with the last entry's name_ set to NULL.
typedef struct CoordInputVars {
  const char *name_;
  CoordCallback *callback_;
} CoordInputVars;

// A text input widget that understands simple math.
class CoordInput : public Fl_Input
{
  Fl_Callback *user_callback_;
  CoordInputVars *vars_;
  void *vars_user_data_;
  static void callback_handler_cb(CoordInput *This, void *v);
  void callback_handler(void *v);
  int eval_var(uchar *&s) const;
  int eval(uchar *&s, int prio) const;
  int eval(const char *s) const;

public:
  CoordInput(int x, int y, int w, int h, const char *l=0L);

  /** Return the text in the widget text field. */
  const char *text() const { return Fl_Input::value(); }

  /** Set the text in the text field */
  void text(const char *v) { Fl_Input::value(v); }

  int value() const;
  void value(int v);

  /** Set the general callback for this widget. */
  void callback(Fl_Callback *cb) {
    user_callback_ = cb;
  }

  /** Set the list of the available variables
   \param vars array of variables, last entry `has name_` set to `NULL`
   \param user_data is forwarded to the Variable callback */
  void variables(CoordInputVars *vars, void *user_data) {
    vars_ = vars;
    vars_user_data_ = user_data;
  }

  int handle(int) FL_OVERRIDE;
};

} // namespace widget

} // namespace fluid

#endif // FLUID_WIDGETS_COORD_INPUT_H
