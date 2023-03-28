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

#include <FL/Fl_Window.H>

class Flpy_Window;
class Flpy_Derived_Window;

typedef struct {
  FlpyObject_HEAD
  Flpy_Window *o;
} Flpy_Window_Object;

class Flpy_Window : public Fl_Window {
public:
  Flpy_Window(int w, int h) : Fl_Window(w, h) { }
  Flpy_Window(int x, int y, int w, int h) : Fl_Window(x, y, w, h) { }
  static int flpy_init(Flpy_Window_Object *self, PyObject *args, PyObject*);
  static PyObject *flpy_show(Flpy_Window_Object *self, PyObject *args) {
    self->o->show(0, NULL);
    Py_RETURN_NONE;
  }
};

int Flpy_Window::flpy_init(Flpy_Window_Object *self, PyObject *args, PyObject*) {
  int x, y, w, h;
  char *label_ = NULL;
  int nargs = (int)PyTuple_Size(args);
  if (nargs <= 3) {
    if (!PyArg_ParseTuple(args, "ii|z", &w, &h, &label_)) return -1;
    self->o = new Flpy_Window(w, h);
  } else {
    if (!PyArg_ParseTuple(args, "iiii|z", &x, &y, &w, &h, &label_)) return -1;
    self->o = new Flpy_Window(x, y, w, h);
  }
  Flpy_Window *o = self->o;
  if (label_) o->copy_label(label_);
  o->user_data(self);
  if (o->parent()) Py_INCREF(self);
  return 0;
}

static PyMethodDef flpy_window_methods[] = {
  { "show", (PyCFunction)Flpy_Window::flpy_show, METH_VARARGS },
  { NULL }
};

PyTypeObject flpy_window_type = {
  .ob_base = { PyObject_HEAD_INIT(NULL) },
  .tp_name = "fltk.Fl_Window",
  .tp_basicsize = sizeof(Flpy_Window_Object),
  .tp_itemsize = 0,
  .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, // how will we now if it is used to generate a new class?
  .tp_doc = PyDoc_STR("Fl_Window"),
  .tp_methods = flpy_window_methods,
  .tp_base = &flpy_group_type,
  .tp_init = (initproc)Flpy_Window::flpy_init,
  .tp_new = PyType_GenericNew,
};
