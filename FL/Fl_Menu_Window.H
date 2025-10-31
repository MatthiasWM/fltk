//
// Menu window header file for the Fast Light Tool Kit (FLTK).
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

/* \file
   Fl_Menu_Window widget . */

#ifndef Fl_Menu_Window_H
#define Fl_Menu_Window_H

#include "Fl_Single_Window.H"

#include <list>

class Fl_Menu_;

/**
  The Fl_Menu_Window widget is a window type used for menus. By
  default the window is drawn in the hardware overlay planes if they are
  available so that the menu don't force the rest of the window to
  redraw.
*/
class FL_EXPORT Fl_Menu_Window : public Fl_Single_Window {
  typedef Fl_Single_Window super;

  Fl_Group *ctor_current_group_ { nullptr };
  bool initialized_ { false };


  static std::list<Fl_Menu_Window*> open_windows_;
  static void register_open_window_(Fl_Menu_Window *win);
  static void deregister_closed_window_(Fl_Menu_Window *win);
  static int app_event_handler_(int event);

public:
  ~Fl_Menu_Window();
  /** Create a menu window and lay it out later. */
  Fl_Menu_Window();
  /** Creates a new Fl_Menu_Window widget using the given size, and label string. */
  Fl_Menu_Window(int W, int H, const char *l = 0);
  /** Creates a new Fl_Menu_Window widget using the given position, size, and label string. */
  Fl_Menu_Window(int X, int Y, int W, int H, const char *l = 0);
  /** End the menu window creation and restore the old current group. */

  void end();

  virtual void show() override;
  virtual void hide() override;

  void init();

  void init_position(const Fl_Menu_* pbutton);

  void layout();

  void pulldown(int X, int Y, int W, int H,
                const Fl_Menu_Item* picked = 0,
                const Fl_Menu_* = 0,
                const Fl_Menu_Item* title = 0,
                int menubar=0);

  virtual int handle(int e) override;

};

#endif
