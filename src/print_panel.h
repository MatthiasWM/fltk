//
// Print panel for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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

//
// This is a temporary file.  It is only for development and will
// probably be removed later.
//

#ifndef print_panel_h
#define print_panel_h
#include <fltk3/Fl.H>
#include <fltk3/Fl_Double_Window.H>
#include <fltk3/Fl_Group.H>
#include <fltk3/Fl_Choice.H>
#include <fltk3/Button.H>
#include <fltk3/Fl_Box.H>
#include <fltk3/Fl_Round_Button.H>
#include <fltk3/Fl_Input.H>
#include <fltk3/Fl_Spinner.H>
#include <fltk3/Fl_Check_Button.H>
#include <fltk3/Fl_Return_Button.H>
#include <fltk3/Fl_Progress.H>
enum printing_style {SystemV, BSD};
static Fl_Double_Window* make_print_panel();
static void print_cb(Fl_Return_Button *, void *);
static printing_style print_load();
static void print_update_status();
#endif
