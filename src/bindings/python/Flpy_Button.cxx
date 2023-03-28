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
  static PyObject *flpy_value(Flpy_Button_Object *self, PyObject *args) {
    int v;
    if (PyTuple_Size(args) == 0) {
      return PyLong_FromLong(self->o->value());
    } else if (PyArg_ParseTuple(args, "i", &v)) {
      return PyLong_FromLong(self->o->value(v));
    }
    return NULL;
  }
  static PyObject *flpy_draw(Flpy_Button_Object *self, PyObject *args) {
    self->o->Fl_Button::draw();
    Py_RETURN_NONE;
  }
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

PyMethodDef flpy_button_methods[] = {
  { "value", (PyCFunction)Flpy_Button::flpy_value, METH_VARARGS },
  { "draw", (PyCFunction)Flpy_Button::flpy_draw, METH_NOARGS },
/*
   Fl_Button(int X, int Y, int W, int H, const char *L = 0);
   void simulate_key_action();
   void draw() FL_OVERRIDE;
   int handle(int) FL_OVERRIDE;
   int value(int v);
   char value() const {return value_;}
   int set() {return value(1);}
   int clear() {return value(0);}
   void setonly(); // this should only be called on FL_RADIO_BUTTONs
   int shortcut() const {return shortcut_;}
   void shortcut(int s) {shortcut_ = s;}
   Fl_Boxtype down_box() const {return (Fl_Boxtype)down_box_;}
   void down_box(Fl_Boxtype b) {down_box_ = b;}
   void shortcut(const char *s) {shortcut(fl_old_shortcut(s));}
   Fl_Color down_color() const {return selection_color();}
   void down_color(unsigned c) {selection_color(c);}
*/
  { NULL }
};

PyTypeObject flpy_button_type = {
  .ob_base = { PyObject_HEAD_INIT(NULL) },
  .tp_name = "fltk.Fl_Button",
  .tp_basicsize = sizeof(Flpy_Button_Object),
  .tp_itemsize = 0,
  .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
  .tp_doc = PyDoc_STR("Fl_Button"),
  .tp_methods = flpy_button_methods,
  .tp_base = &flpy_widget_type,
  .tp_init = (initproc)Flpy_Button::flpy_init,
  .tp_new = PyType_GenericNew,
};
