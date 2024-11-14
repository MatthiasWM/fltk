//
// Widget Bin Button Classes for Fast Light User Interface Designer (FLUID).
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

#include "widgets/bin_buttons.h"

#include "fluid.h"
#include "Fl_Window_Type.h"
#include "factory.h"
#include "widget_panel.h"
#include "widget_browser.h"

#include <FL/platform.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Menu_.H>
#include <FL/fl_string_functions.h>
#include "../src/flstring.h"

using namespace fluid;

/** \class widget::BinButton
 The widget::BinButton button is a button that can be used in the widget bin to
 allow the user to drag and drop widgets into a window or group. This feature
 makes it easy for the user to position a widget at a specific location within
 the window or group.
 */

/**
 Convert mouse dragging into a drag and drop event.
 */
int widget::BinButton::handle(int inEvent)
{
  int ret = 0;
  switch (inEvent) {
    case FL_PUSH:
      Fl_Button::handle(inEvent);
      return 1; // make sure that we get drag events
    case FL_DRAG:
      ret = Fl_Button::handle(inEvent);
      if (!user_data())
        return ret;
      if (!Fl::event_is_click()) { // make it a dnd event
        // fake a drag outside of the widget
        Fl::e_x = x()-1;
        Fl_Button::handle(inEvent);
        // fake a button release
        Fl_Button::handle(FL_RELEASE);
        // make it into a dnd event
        const char *type_name = (const char*)user_data();
        Fl_Type::current_dnd = Fl_Type::current;
        Fl::copy(type_name, (int)strlen(type_name)+1, 0);
        Fl::dnd();
        return 1;
      }
      return ret;
  }
  return Fl_Button::handle(inEvent);
}

/** \class widget::BinWindowButton
 The widget::BinWindowButton button is used in the widget bin to create new
 windows by dragging and dropping. When the button is dragged and dropped onto
 the desktop, a new window will be created at the drop location.
 */

/**
 Convert mouse dragging into a drag and drop event.
 */
int widget::BinWindowButton::handle(int inEvent)
{
  static Fl_Window *drag_win = NULL;
  int ret = 0;
  switch (inEvent) {
    case FL_PUSH:
      Fl_Button::handle(inEvent);
      return 1; // make sure that we get drag events
    case FL_DRAG:
      ret = Fl_Button::handle(inEvent);
      if (!user_data())
        return ret;
      if (!Fl::event_is_click()) {
        if (!drag_win) {
          drag_win = new Fl_Window(0, 0, 480, 320);
          drag_win->border(0);
          drag_win->set_non_modal();
        }
        if (drag_win) {
          drag_win->position(Fl::event_x_root()+1, Fl::event_y_root()+1);
          drag_win->show();
        }
        // Does not work outside window: fl_cursor(FL_CURSOR_HAND);
      }
      return ret;
    case FL_RELEASE:
      if (drag_win) {
        Fl::delete_widget(drag_win);
        drag_win = NULL;
        // create a new window here
        Fl_Type *prototype = typename_to_prototype((char*)user_data());
        if (prototype) {
          Fl_Type *new_type = add_new_widget_from_user(prototype, kAddAfterCurrent);
          if (new_type && new_type->is_a(ID_Window)) {
            Fl_Window_Type *new_window = (Fl_Window_Type*)new_type;
            Fl_Window *w = (Fl_Window *)new_window->o;
            w->position(Fl::event_x_root(), Fl::event_y_root());
          }
        }
        widget_browser->display(Fl_Type::current);
        widget_browser->rebuild();
      }
      return Fl_Button::handle(inEvent);
  }
  return Fl_Button::handle(inEvent);
}

