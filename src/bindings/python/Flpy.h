//
// Python bindings for the Fast Light Tool Kit (FLTK).
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

#ifndef _FLPY_H_
#define _FLPY_H_ 1

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <FL/Fl_Export.H>

extern PyTypeObject flpy_type;
extern PyTypeObject flpy_widget_type;
extern PyTypeObject flpy_group_type;
extern PyTypeObject flpy_window_type;
extern PyTypeObject flpy_button_type;

#define FlpyObject_HEAD \
  PyObject_HEAD \
  PyObject *callback_class; \
  PyObject *callback_method; \
  PyObject *callback_data;

extern void flpy_on_callback(class Fl_Widget *w, void *v);

#endif


