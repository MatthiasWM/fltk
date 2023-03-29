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

#include <FL/Fl_Button.H>

class Flpy_Button;
class Flpy_Derived_Button;

typedef struct {
  FlpyObject_HEAD
  Flpy_Button *o;
} Flpy_Button_Object;

class Flpy_Button : public Fl_Button {
public:
  Flpy_Button(int x, int y, int w, int h) : Fl_Button(x, y, w, h) { }
  static int flpy_init(Flpy_Button_Object *self, PyObject *args, PyObject*);
  static PyMethodDef flpy_methods[];
};

class Flpy_Derived_Button : public Flpy_Button {
public:
  Flpy_Derived_Button(int x, int y, int w, int h) : Flpy_Button(x, y, w, h) { }
  void draw() FL_OVERRIDE {
    Flpy_Derived_Button *self = (Flpy_Derived_Button*)user_data();
    PyObject_CallMethodNoArgs((PyObject*)self, PyUnicode_FromString("draw"));
  }
};

int Flpy_Button::flpy_init(Flpy_Button_Object *self, PyObject *args, PyObject*) {
  int x, y, w, h;
  char *label_ = NULL;
  if (!PyArg_ParseTuple(args, "iiii|z", &x, &y, &w, &h, &label_)) return -1;
  if (Py_TYPE(self) == &flpy_type)
    self->o = new Flpy_Button(x, y, w, h);
  else
    self->o = new Flpy_Derived_Button(x, y, w, h);
  Flpy_Button *o = self->o;
  if (label_) o->copy_label(label_);
  o->callback(flpy_on_callback, self);
  if (o->parent()) Py_INCREF(self);
  return 0;
}

PyMethodDef Flpy_Button::flpy_methods[] = {
  FlpyMETHOD_VARARGS_BEGIN(Button, value)
    FlpyARG_i_TO_i(Button, value)
    FlpyARG_v_TO_i(Button, value)
  FlpyMETHOD_VARARGS_END(Button, value, "takes no arguments or one integer"),
  FlpyMETHOD_VIRT_v_TO_v(Button, draw),
  FlpyMETHOD_VIRT_i_TO_i(Button, handle),
  FlpyMETHOD_v_TO_v(Button, simulate_key_action),
  FlpyMETHOD_v_TO_v(Button, set),
  FlpyMETHOD_v_TO_v(Button, clear),
  FlpyMETHOD_v_TO_v(Button, setonly),
  FlpyMETHOD_VARARGS_BEGIN(Button, shortcut)
    FlpyARG_i_TO_v(Button, shortcut)
    FlpyARG_z_TO_v(Button, shortcut)
    FlpyARG_v_TO_i(Button, shortcut)
  FlpyMETHOD_VARARGS_END(Button, shortcut, "takes no arguments or a shortcut keycode"),
  FlpyMETHOD_VARARGS_BEGIN(Button, down_box)
    FlpyARG_I_TO_v_TYPE(Button, down_box, Fl_Boxtype)
    FlpyARG_v_TO_i(Button, down_box)
  FlpyMETHOD_VARARGS_END(Button, down_box, "takes no arguments or a boxtype"),
  FlpyMETHOD_VARARGS_BEGIN(Button, down_color)
    FlpyARG_I_TO_v_TYPE(Button, down_color, Fl_Color)
    FlpyARG_v_TO_i(Button, down_color)
  FlpyMETHOD_VARARGS_END(Button, down_color, "takes no arguments or one color"),
  { NULL }
};

PyTypeObject flpy_button_type = {
  .ob_base = { PyObject_HEAD_INIT(NULL) },
  .tp_name = "fltk.Fl_Button",
  .tp_basicsize = sizeof(Flpy_Button_Object),
  .tp_itemsize = 0,
  .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
  .tp_doc = PyDoc_STR("Fl_Button"),
  .tp_methods = Flpy_Button::flpy_methods,
  .tp_base = &flpy_widget_type,
  .tp_init = (initproc)Flpy_Button::flpy_init,
  .tp_new = PyType_GenericNew,
};
