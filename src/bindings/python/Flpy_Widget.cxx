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

#include <FL/Fl_Widget.H>
#include <FL/Fl_Box.H>

class Flpy_Widget;
class Flpy_Derived_Widget;

typedef struct {
  FlpyObject_HEAD
  Flpy_Widget *o;
} Flpy_Object_Widget;

class Flpy_Widget : public Fl_Box {
public:
  Flpy_Widget(int x, int y, int w, int h) : Fl_Box(x, y, w, h) {
  }
  static PyObject *flpy_show(Flpy_Object_Widget *self, PyObject *args) {
    self->o->show();
    Py_RETURN_NONE;
  }
  static PyObject *flpy_label(Flpy_Object_Widget *self, PyObject *args) {
    char *str = NULL;
    if (PyTuple_Size(args) == 0) {
      // This crashes when the label is NULL
      if (self->o->label() == NULL) Py_RETURN_NONE;
      return PyUnicode_FromString(self->o->label());
    } else if (PyArg_ParseTuple(args, "z", &str)) {
      self->o->copy_label(str);
      Py_RETURN_NONE;
    }
    return NULL;
  }
  static PyObject *flpy_callback(Flpy_Object_Widget *self, PyObject *args) {
    if (PyTuple_Size(args) == 0) {
      return self->callback_method;
    } else {
      self->callback_method = PyTuple_GetItem(args, 0);
      int v = PyCallable_Check(self->callback_method);
      Py_INCREF(self->callback_method);
      if (PyTuple_Size(args) == 1) {
        self->callback_data = Py_None;
      } else {
        self->callback_data = PyTuple_GetItem(args, 1);
        Py_XINCREF(self->callback_data);
      }
      Py_RETURN_NONE;
    }
    return NULL;
  }
  static int flpy_init(Flpy_Object_Widget *self, PyObject *args, PyObject*) {
    int x, y, w, h;
    if (!PyArg_ParseTuple(args, "iiii", &x, &y, &w, &h)) return -1;
    Fl_Widget *o = self->o = new Flpy_Widget(x, y, w, h);
    o->callback(flpy_on_callback, self);
    if (o->parent()) Py_INCREF(self);
    return 0;
  }
};

class Flpy_Derived_Widget : public Flpy_Widget {
public:
  Flpy_Derived_Widget(int x, int y, int w, int h) : Flpy_Widget(x, y, w, h) { }
};

static PyMethodDef flpy_widget_methods[] = {
  { "show", (PyCFunction)Flpy_Widget::flpy_show, METH_NOARGS },
  { "label", (PyCFunction)Flpy_Widget::flpy_label, METH_VARARGS },
  { "callback", (PyCFunction)Flpy_Widget::flpy_callback, METH_VARARGS },
  { NULL }
};

PyTypeObject flpy_widget_type = {
  PyObject_HEAD_INIT(NULL)
    .tp_name = "fltk.Fl_Widget",
    .tp_doc = PyDoc_STR("Fl_Widget"),
    .tp_basicsize = sizeof(Flpy_Object_Widget),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, // how will we now if it is used to generate a new class?
    .tp_new = PyType_GenericNew,
    .tp_init = (initproc)Flpy_Widget::flpy_init,
    .tp_methods = flpy_widget_methods,
};

void flpy_on_callback(Fl_Widget *w, void *v) {
  Flpy_Object_Widget *self = (Flpy_Object_Widget*)v;
  if (self->callback_method) {
    // if callback_obj is set, call the method in an object with self, widget, user_data
    // else call the function with widget, user_data
    // NOTE: if we allow an arbitrary number of parameters in Fl_Widget.callback(),
    // we can create a tuple here with an arbitray number of parameters, and so the callback can have that too.
    PyObject *tuple = PyTuple_Pack(1, self->callback_data);
    Py_INCREF(tuple);
    PyObject *result = PyObject_CallObject(self->callback_method, tuple); //self->callback_data);
    Py_DECREF(tuple);
  }
  // NOTE: we could also test if self.on_callback exists and call that. Would that be easier?
}

