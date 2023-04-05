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

extern PyObject *flpy_get_int(PyObject *, void *v);

#define Flpy_GET_INT(NAME) { #NAME, flpy_get_int, NULL, NULL, (void*)(NAME) }

#define FlpyObject_HEAD \
  PyObject_HEAD \
  PyObject *callback_method; \
  PyObject *callback_data;

typedef struct {
  FlpyObject_HEAD
  class Fl_Widget *o;
} Flpy_Fl_Widget_Object;

typedef struct {
  FlpyObject_HEAD
  class Fl_Group *o;
} Flpy_Fl_Group_Object;

extern void flpy_on_callback(class Fl_Widget *w, void *v);

#define FlpyARG_IiiiiI_TO_v_TYPE(KLASS, NAME, TYPE0, TYPE5) if (PyTuple_GET_SIZE(args)==6 && PyLong_Check(PyTuple_GET_ITEM(args, 0)) && PyLong_Check(PyTuple_GET_ITEM(args, 1)) && PyLong_Check(PyTuple_GET_ITEM(args, 2)) && PyLong_Check(PyTuple_GET_ITEM(args, 3)) && PyLong_Check(PyTuple_GET_ITEM(args, 4)) && PyLong_Check(PyTuple_GET_ITEM(args, 5))) { ((Flpy_##KLASS##_Object*)self)->o->NAME((TYPE0)PyLong_AsUnsignedLong(PyTuple_GET_ITEM(args, 0)), (int)PyLong_AsLong(PyTuple_GET_ITEM(args, 1)), (int)PyLong_AsLong(PyTuple_GET_ITEM(args, 2)), (int)PyLong_AsLong(PyTuple_GET_ITEM(args, 3)), (int)PyLong_AsLong(PyTuple_GET_ITEM(args, 4)), (TYPE5)PyLong_AsUnsignedLong(PyTuple_GET_ITEM(args, 5))); Py_RETURN_NONE; }
#define FlpyARG_iiiiI_TO_v_TYPE(KLASS, NAME, TYPE4) if (PyTuple_GET_SIZE(args)==5 && PyLong_Check(PyTuple_GET_ITEM(args, 0)) && PyLong_Check(PyTuple_GET_ITEM(args, 1)) && PyLong_Check(PyTuple_GET_ITEM(args, 2)) && PyLong_Check(PyTuple_GET_ITEM(args, 3)) && PyLong_Check(PyTuple_GET_ITEM(args, 4))) { ((Flpy_##KLASS##_Object*)self)->o->NAME((int)PyLong_AsLong(PyTuple_GET_ITEM(args, 0)), (int)PyLong_AsLong(PyTuple_GET_ITEM(args, 1)), (int)PyLong_AsLong(PyTuple_GET_ITEM(args, 2)), (int)PyLong_AsLong(PyTuple_GET_ITEM(args, 3)), (TYPE4)PyLong_AsUnsignedLong(PyTuple_GET_ITEM(args, 4))); Py_RETURN_NONE; }
#define FlpyARG_Iiiii_TO_v_TYPE(KLASS, NAME, TYPE0) if (PyTuple_GET_SIZE(args)==5 && PyLong_Check(PyTuple_GET_ITEM(args, 0)) && PyLong_Check(PyTuple_GET_ITEM(args, 1)) && PyLong_Check(PyTuple_GET_ITEM(args, 2)) && PyLong_Check(PyTuple_GET_ITEM(args, 3)) && PyLong_Check(PyTuple_GET_ITEM(args, 4))) { ((Flpy_##KLASS##_Object*)self)->o->NAME((TYPE0)PyLong_AsUnsignedLong(PyTuple_GET_ITEM(args, 0)), (int)PyLong_AsLong(PyTuple_GET_ITEM(args, 1)), (int)PyLong_AsLong(PyTuple_GET_ITEM(args, 2)), (int)PyLong_AsLong(PyTuple_GET_ITEM(args, 3)), (int)PyLong_AsLong(PyTuple_GET_ITEM(args, 4))); Py_RETURN_NONE; }
#define FlpyARG_iiii_TO_v(KLASS, NAME) if (PyTuple_GET_SIZE(args)==4 && PyLong_Check(PyTuple_GET_ITEM(args, 0)) && PyLong_Check(PyTuple_GET_ITEM(args, 1)) && PyLong_Check(PyTuple_GET_ITEM(args, 2)) && PyLong_Check(PyTuple_GET_ITEM(args, 3))) { ((Flpy_##KLASS##_Object*)self)->o->NAME((int)PyLong_AsUnsignedLong(PyTuple_GET_ITEM(args, 0)), (int)PyLong_AsLong(PyTuple_GET_ITEM(args, 1)), (int)PyLong_AsLong(PyTuple_GET_ITEM(args, 2)), (int)PyLong_AsLong(PyTuple_GET_ITEM(args, 3))); Py_RETURN_NONE; }
#define FlpyARG_II_TO_v_TYPE(KLASS, NAME, TYPE0, TYPE1) if (PyTuple_GET_SIZE(args)==2 && PyLong_Check(PyTuple_GET_ITEM(args, 0)) && PyLong_Check(PyTuple_GET_ITEM(args, 1))) { ((Flpy_##KLASS##_Object*)self)->o->NAME((TYPE0)PyLong_AsUnsignedLong(PyTuple_GET_ITEM(args, 0)), (TYPE1)PyLong_AsUnsignedLong(PyTuple_GET_ITEM(args, 1))); Py_RETURN_NONE; }
#define FlpyARG_I_TO_v_TYPE(KLASS, NAME, TYPE0) if (PyTuple_GET_SIZE(args)==1 && PyLong_Check(PyTuple_GET_ITEM(args, 0))) { ((Flpy_##KLASS##_Object*)self)->o->NAME((TYPE0)PyLong_AsUnsignedLong(PyTuple_GET_ITEM(args, 0))); Py_RETURN_NONE; }

#define FlpyARG_v_TO_i(KLASS, NAME) if (PyTuple_GET_SIZE(args)==0) { return PyLong_FromLong( ((Flpy_##KLASS##_Object*)self)->o->NAME() ); }
#define FlpyARG_i_TO_v(KLASS, NAME) if (PyTuple_GET_SIZE(args)==1 && PyLong_Check(PyTuple_GET_ITEM(args, 0))) { ((Flpy_##KLASS##_Object*)self)->o->NAME((int)PyLong_AsLong(PyTuple_GET_ITEM(args, 0))); Py_RETURN_NONE; }
#define FlpyARG_i_TO_i(KLASS, NAME) if (PyTuple_GET_SIZE(args)==1 && PyLong_Check(PyTuple_GET_ITEM(args, 0))) { return PyLong_FromLong( ((Flpy_##KLASS##_Object*)self)->o->NAME((int)PyLong_AsLong(PyTuple_GET_ITEM(args, 0))) ); }

#define FlpyARG_z_TO_v(KLASS, NAME) if (PyTuple_GET_SIZE(args)==1 && PyUnicode_Check(PyTuple_GET_ITEM(args, 0))) { ((Flpy_##KLASS##_Object*)self)->o->NAME(PyUnicode_AsUTF8(PyTuple_GET_ITEM(args, 0))); Py_RETURN_NONE; }
#define FlpyARG_v_TO_z(KLASS, NAME) {} if (PyTuple_GET_SIZE(args)==0) { const char *str = ((Flpy_##KLASS##_Object*)self)->o->NAME(); return str ? PyUnicode_FromString(str) : Py_NewRef(Py_None); }

#define FlpyARG_v_TO_w(KLASS, NAME) if (PyTuple_GET_SIZE(args)==0) { Fl_Widget *w = ((Flpy_##KLASS##_Object*)self)->o->NAME(); if (w) return (PyObject*)w->user_data(); Py_RETURN_NONE; }
#define FlpyARG_g_TO_v(KLASS, NAME) if (PyTuple_GET_SIZE(args)==1 && PyObject_TypeCheck(PyTuple_GET_ITEM(args, 0), &flpy_group_type)) { ((Flpy_##KLASS##_Object*)self)->o->NAME(((Flpy_Fl_Group_Object*)PyTuple_GET_ITEM(args, 0))->o); Py_RETURN_NONE; }
#define FlpyARG_w_TO_v(KLASS, NAME) if (PyTuple_GET_SIZE(args)==1 && PyObject_TypeCheck(PyTuple_GET_ITEM(args, 0), &flpy_widget_type)) { ((Flpy_##KLASS##_Object*)self)->o->NAME(((Flpy_Fl_Widget_Object*)PyTuple_GET_ITEM(args, 0))->o); Py_RETURN_NONE; }
#define FlpyARG_w_TO_i(KLASS, NAME) if (PyTuple_GET_SIZE(args)==1 && PyObject_TypeCheck(PyTuple_GET_ITEM(args, 0), &flpy_widget_type)) { return PyLong_FromLong(((Flpy_##KLASS##_Object*)self)->o->NAME(((Flpy_Fl_Widget_Object*)PyTuple_GET_ITEM(args, 0))->o)); }

#define FlpyARG_v_TO_v(KLASS, NAME) if (PyTuple_GET_SIZE(args)==0) { ((Flpy_##KLASS##_Object*)self)->o->NAME(); Py_RETURN_NONE; }

#define FlpyMETHOD_VARARGS_BEGIN(KLASS, NAME) { #NAME, [](PyObject *self, PyObject *args)->PyObject* {
#define FlpyMETHOD_VARARGS_END(KLASS, NAME, MSG) PyErr_SetString(PyExc_TypeError, "Fl_"#KLASS"."#NAME"() " MSG); return NULL; }, METH_VARARGS }
#define FlpyMETHOD_v_TO_v(KLASS, NAME) { #NAME, [](PyObject *self, PyObject *args)->PyObject* { ((Flpy_##KLASS##_Object*)self)->o->NAME(); Py_RETURN_NONE; }, METH_NOARGS }
#define FlpyMETHOD_v_TO_i(KLASS, NAME) { #NAME, [](PyObject *self, PyObject *args)->PyObject* { return PyLong_FromLong(((Flpy_##KLASS##_Object*)self)->o->NAME()); }, METH_NOARGS }
#define FlpyMETHOD_v_TO_I(KLASS, NAME) { #NAME, [](PyObject *self, PyObject *args)->PyObject* { return PyLong_FromUnsignedLong(((Flpy_##KLASS##_Object*)self)->o->NAME()); }, METH_NOARGS }
#define FlpyMETHOD_v_TO_w(KLASS, NAME) { #NAME, [](PyObject *self, PyObject *args)->PyObject* { Fl_Widget *w = ((Flpy_##KLASS##_Object*)self)->o->NAME(); if (w) return (PyObject*)w->user_data(); Py_RETURN_NONE; }, METH_NOARGS }
#define FlpyMETHOD_I_TO_v(KLASS, NAME) { #NAME, [](PyObject *self, PyObject *args)->PyObject* { unsigned int arg0; if (!PyArg_ParseTuple(args, "I", &arg0)) return NULL; ((Flpy_##KLASS##_Object*)self)->o->NAME(arg0); Py_RETURN_NONE; }, METH_VARARGS }
#define FlpyMETHOD_I0_TO_v(KLASS, NAME) { #NAME, [](PyObject *self, PyObject *args)->PyObject* { unsigned int arg0=0; if (!PyArg_ParseTuple(args, "|I", &arg0)) return NULL; ((Flpy_##KLASS##_Object*)self)->o->NAME(arg0); Py_RETURN_NONE; }, METH_VARARGS }
#define FlpyMETHOD_ii_TO_v(KLASS, NAME) { #NAME, [](PyObject *self, PyObject *args)->PyObject* { int arg0, arg1; if (!PyArg_ParseTuple(args, "ii", &arg0, &arg1)) return NULL; ((Flpy_##KLASS##_Object*)self)->o->NAME(arg0, arg1); Py_RETURN_NONE; }, METH_VARARGS }
#define FlpyMETHOD_iiii_TO_i(KLASS, NAME) { #NAME, [](PyObject *self, PyObject *args)->PyObject* { int arg0, arg1, arg2, arg3; if (!PyArg_ParseTuple(args, "iiii", &arg0, &arg1, &arg2, &arg3)) return NULL; return PyLong_FromLong(((Flpy_##KLASS##_Object*)self)->o->NAME(arg0, arg1, arg2, arg3)); }, METH_VARARGS }

#define FlpyMETHOD_VIRT_v_TO_v(KLASS, NAME) { #NAME, [](PyObject *self, PyObject *args)->PyObject* { ((Flpy_##KLASS##_Object*)self)->o->Flpy_##KLASS::NAME(); Py_RETURN_NONE; }, METH_NOARGS }
#define FlpyMETHOD_VIRT_i_TO_i(KLASS, NAME) { #NAME, [](PyObject *self, PyObject *args)->PyObject* { int arg0; if (!PyArg_ParseTuple(args, "i", &arg0)) return NULL; return PyLong_FromLong(((Flpy_##KLASS##_Object*)self)->o->Flpy_##KLASS::NAME(arg0)); }, METH_VARARGS }
#define FlpyMETHOD_VIRT_iiii_TO_v(KLASS, NAME) { #NAME, [](PyObject *self, PyObject *args)->PyObject* { int arg0, arg1, arg2, arg3; if (!PyArg_ParseTuple(args, "iiii", &arg0, &arg1, &arg2, &arg3)) return NULL; ((Flpy_##KLASS##_Object*)self)->o->Flpy_##KLASS::NAME(arg0, arg1, arg2, arg3); Py_RETURN_NONE; }, METH_VARARGS }

#define FlpyTYPE(KLASS, klass, base, DOCS) \
PyTypeObject flpy_##klass##_type = { \
  PyObject_HEAD_INIT(NULL) \
  "fltk.Fl_"#KLASS,               /* tp_name */ \
  sizeof(Flpy_##KLASS##_Object),  /* tp_basicsize */ \
  0,                              /* tp_itemsize */ \
  (destructor)Flpy_##KLASS::flpy_dealloc, /* tp_dealloc */ \
  0,                              /* tp_vectorcall_offset */ \
  0,                              /* tp_getattr */ \
  0,                              /* tp_setattr */ \
  0,                              /* tp_as_async */ \
  NULL,                           /* tp_repr */ \
  0,                              /* tp_as_number */ \
  0,                              /* tp_as_sequence */ \
  0,                              /* tp_as_mapping */ \
  0,                              /* tp_hash */ \
  0,                              /* tp_call */ \
  0,                              /* tp_str */ \
  0,                              /* tp_getattro */ \
  0,                              /* tp_setattro */ \
  0,                              /* tp_as_buffer */ \
  Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */ \
  PyDoc_STR(DOCS),                /* tp_doc */ \
  0,                              /* tp_traverse */ \
  0,                              /* tp_clear */ \
  0,                              /* tp_richcompare */ \
  0,                              /* tp_weaklistoffset */ \
  0,                              /* tp_iter */ \
  0,                              /* tp_iternext */ \
  Flpy_##KLASS::flpy_methods,     /* tp_methods */ \
  0,                              /* tp_members */ \
  Flpy_##KLASS::flpy_getset,      /* tp_getset */ \
  base,                           /* tp_base */ \
  0,                              /* tp_dict */ \
  0,                              /* tp_descr_get */ \
  0,                              /* tp_descr_set */ \
  0,                              /* tp_dictoffset */ \
  (initproc)Flpy_##KLASS::flpy_init, /* tp_init */ \
  0,                              /* tp_alloc */ \
  PyType_GenericNew,              /* tp_new */ \
};


#endif


