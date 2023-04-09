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
    // Fl_Window.show(sys.argv)
    self->o->show(0, NULL);
    Py_RETURN_NONE;
  }
  static void flpy_dealloc(Flpy_Window_Object *self) {
    PyTypeObject *tp = Py_TYPE(self);
    printf("Window flpy_dealloc\n");
    // TODO: free references and buffers here
    tp->tp_free(self);
  }
  static PyMethodDef flpy_methods[];
  static PyGetSetDef flpy_getset[];
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

PyMethodDef Flpy_Window::flpy_methods[] = {
  { "show", (PyCFunction)flpy_show, METH_VARARGS },
  { NULL }
};

PyGetSetDef Flpy_Window::flpy_getset[] = {
  { NULL }
};

FlpyTYPE(Window, window, &flpy_group_type, "This widget produces a window on the desktop.")
