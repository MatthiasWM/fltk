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

#include <FL/Fl_Group.H>

class Flpy_Group;
class Flpy_Derived_Group;

typedef struct {
  FlpyObject_HEAD
  Flpy_Group *o;
} Flpy_Group_Object;

class Flpy_Group : public Fl_Group {
public:
  Flpy_Group(int x, int y, int w, int h) : Fl_Group(x, y, w, h) { }
  static int flpy_init(Flpy_Group_Object *self, PyObject *args, PyObject*);
  static PyObject *flpy_show(Flpy_Group_Object *self, PyObject *args) {
    self->o->show();
    Py_RETURN_NONE;
  }
};

class Flpy_Derived_Group : public Flpy_Group {
public:
  Flpy_Derived_Group(int x, int y, int w, int h) : Flpy_Group(x, y, w, h) { }
};

int Flpy_Group::flpy_init(Flpy_Group_Object *self, PyObject *args, PyObject*) {
  int x, y, w, h;
  char *label_ = NULL;
  if (!PyArg_ParseTuple(args, "iiii|z", &x, &y, &w, &h, &label_)) return -1;
  if (Py_TYPE(self) == &flpy_type)
    self->o = new Flpy_Group(x, y, w, h);
  else
    self->o = new Flpy_Derived_Group(x, y, w, h);
  Flpy_Group *o = self->o;
  if (label_) o->copy_label(label_);
  o->user_data(self);
  if (o->parent()) Py_INCREF(self);
  return 0;
}

static PyMethodDef flpy_group_methods[] = {
  { "show", (PyCFunction)Flpy_Group::flpy_show, METH_VARARGS },
  { NULL }
};

PyTypeObject flpy_group_type = {
  .ob_base = { PyObject_HEAD_INIT(NULL) },
  .tp_name = "fltk.Fl_Group",
  .tp_basicsize = sizeof(Flpy_Group_Object),
  .tp_itemsize = 0,
  .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, // how will we now if it is used to generate a new class?
  .tp_doc = PyDoc_STR("Fl_Group"),
  .tp_methods = flpy_group_methods,
  .tp_base = &flpy_widget_type, // actually flpy_type_group...
  .tp_init = (initproc)Flpy_Group::flpy_init,
  .tp_new = PyType_GenericNew,
};
