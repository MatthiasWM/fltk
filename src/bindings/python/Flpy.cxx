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

#include "Flpy.h"

#include <FL/fl_draw.H>


static PyObject *flpy_color(PyObject *self, PyObject *args)
{
  if (PyTuple_Size(args)==0) {
    return PyLong_FromLong(fl_color());
    //    return Py_BuildValue("I", fl_color());
  }
  unsigned int color;
  if (PyArg_ParseTuple(args, "I", &color)) {
    fl_color(color);
    Py_RETURN_NONE;
  }
  return NULL;
}

static PyObject *flpy_rectf(PyObject *self, PyObject *args)
{
  int x, y, w, h;
  if (PyArg_ParseTuple(args, "iiii", &x, &y, &w, &h)) {
    fl_rectf(x, y, w, h);
    Py_RETURN_NONE;
  }
  return NULL;
}

static PyMethodDef fltk_methods[] = {
  {"fl_color", flpy_color, METH_VARARGS, "Set the current color."},
  {"fl_rectf", flpy_rectf, METH_VARARGS, "Draw a rectangle."},
  {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef fltk_module = {
  PyModuleDef_HEAD_INIT,
  "fltk",
  "FLTK - the fast light GUI toolkit",
  -1,
  fltk_methods
};

extern "C" FL_EXPORT PyObject *PyInit_fltk()
{
  //  if (PyType_Ready(&flpyo_widget) < 0)
  //    return NULL;

  PyObject *m = PyModule_Create(&fltk_module);

  PyType_Ready(&flpy_type);
  PyObject *Flpy_Type = PyObject_New(PyObject, &flpy_type);
  if (PyModule_AddObject(m, "Fl", Flpy_Type) < 0) {
    Py_DECREF(&flpy_widget_type);
    Py_RETURN_NONE;
  }

  PyType_Ready(&flpy_widget_type);
  Py_INCREF(&flpy_widget_type);
  if (PyModule_AddObject(m, "Fl_Widget", (PyObject *)&flpy_widget_type) < 0) {
    Py_DECREF(&flpy_widget_type);
    Py_RETURN_NONE;
  }

  PyType_Ready(&flpy_button_type);
  Py_INCREF(&flpy_button_type);
  if (PyModule_AddObject(m, "Fl_Button", (PyObject *)&flpy_button_type) < 0) {
    Py_DECREF(&flpy_button_type);
    Py_RETURN_NONE;
  }

  PyType_Ready(&flpy_window_type);
  Py_INCREF(&flpy_window_type);
  if (PyModule_AddObject(m, "Fl_Window", (PyObject *)&flpy_window_type) < 0) {
    Py_DECREF(&flpy_window_type);
    Py_RETURN_NONE;
  }

  PyModule_AddIntMacro(m, FL_RED);
  PyModule_AddIntMacro(m, FL_BOLD);
  PyModule_AddIntMacro(m, FL_ITALIC);

  return m;
}
