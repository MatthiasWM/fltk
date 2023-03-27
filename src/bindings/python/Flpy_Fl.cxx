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

#include <FL/Fl.H>

static PyObject *flpy_method_fl_run(PyObject*, PyObject *args) {
  Fl::run();
  Py_RETURN_NONE;
}

static PyMethodDef flpy_methods_fl[] = {
  { "run", (PyCFunction)flpy_method_fl_run, METH_NOARGS|METH_STATIC },
  { NULL }
};

PyObject *flpy_fl_get_int(PyObject *, void *v) {
  return Py_BuildValue("i", (int)(fl_intptr_t)v);
}

static PyGetSetDef flpy_fl_getset[] = {
  { "OPTION_ARROW_FOCUS", flpy_fl_get_int, NULL, NULL, (void*)0 },
  { "OPTION_VISIBLE_FOCUS", flpy_fl_get_int, NULL, NULL, (void*)1 },
  { NULL }
};

PyTypeObject flpy_type = {
  .ob_base = { PyObject_HEAD_INIT(NULL) },
  .tp_name = "fltk.Fl",
  .tp_flags = Py_TPFLAGS_DEFAULT /*|Py_TPFLAGS_DISALLOW_INSTANTIATION*/,
  .tp_doc = PyDoc_STR("Fl"),
  .tp_methods = flpy_methods_fl,
  .tp_getset = flpy_fl_getset,
};

