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
  static void flpy_dealloc(Flpy_Group_Object *self) {
    PyTypeObject *tp = Py_TYPE(self);
    printf("Group flpy_dealloc\n");
    // TODO: free references and buffers here
    tp->tp_free(self);
    Py_DECREF(tp);
  }
  static PyMethodDef flpy_methods[];
  static PyGetSetDef flpy_getset[];
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

PyMethodDef Flpy_Group::flpy_methods[] = {
  { "show", (PyCFunction)flpy_show, METH_VARARGS },
  FlpyMETHOD_VARARGS_BEGIN(Group, current)
    FlpyARG_v_TO_w(Group, current)
    FlpyARG_g_TO_v(Group, current) // not available
  FlpyMETHOD_VARARGS_END(Group, current, "takes no argument, or a group"),
  { NULL }
};

PyGetSetDef Flpy_Group::flpy_getset[] = {
  { NULL }
};

FlpyTYPE(Group, group, &flpy_widget_type, "The Fl_Group class is the FLTK container widget. It maintains an array of child widgets.")
