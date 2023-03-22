//
// Hello, World! program for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2021 by Bill Spitzak and others.
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

//find_package(PythonLibs REQUIRED)
//include_directories(${PYTHON_INCLUDE_DIRS})
//target_link_libraries(<your exe or lib> ${PYTHON_LIBRARIES})
//For details of the commands, run:
//
//cmake --help-module FindPythonLibs
//cmake --help-command find_package
//cmake --help-command include_directories
//cmake --help-command target_link_libraries

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#define protected public

#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>

const char *prg = R"EOF(

from fltk import *

def test_callback(a):
  print("Hallo Welt")
  print(a)
  print(Fl.OPTION_VISIBLE_FOCUS)

class MyButton(Fl_Button):
  def draw(self):
    print(self)
    fl_color(3)
    fl_rectf(10, 10, 150, 100)
    fl_color(4)
    fl_rectf(40, 40, 150, 100)
    super().draw()

window = Fl_Window(340, 180)
#  Fl_Box *box = new Fl_Box(20, 40, 300, 100, "Hello, World!");
#btn = MyButton(20, 40, 300, 100, "Hello, World!")
btn = Fl_Button(20, 40, 300, 100, "Hello, World!")
btn.callback(test_callback, "Yolandi")
#  box->box(FL_UP_BOX);
#  box->labelfont(FL_BOLD + FL_ITALIC);
#  box->labelsize(36);
#  box->labeltype(FL_SHADOW_LABEL);
#  window->end();
#  window->show(argc, argv);
#window.label("Hello!")
window.show()
#  return Fl::run();
Fl.run()

)EOF";


// ---- flpy_window ------------------------------------------------------------

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
  "fltk",   /* name of module */
  "FLTK - the fast light GUI toolkit", /* module documentation, may be NULL */
  -1,       /* size of per-interpreter state of the module,
             or -1 if the module keeps state in global variables. */
  fltk_methods
};

// ---- flpy_fl ------------------------------------------------------------

//typedef struct {
//  PyObject_HEAD
//} Flpy_Object_Fl;

static PyObject *flpy_method_fl_run(PyObject * /*self=NULL*/, PyObject *args) {
  Fl::run();
  Py_RETURN_NONE;
}

static PyMethodDef flpy_methods_fl[] = {
  { "run", (PyCFunction)flpy_method_fl_run, METH_NOARGS|METH_STATIC },
  { NULL }
};

PyObject *flpy_fl_get_e_number(PyObject *, void *) {
  return Py_BuildValue("i", Fl::e_number);
}
int flpy_fl_set_e_number(PyObject *, PyObject *arg, void *) {
  int v = (int)PyLong_AsLong(arg);
  if (PyErr_Occurred()) {
    PyErr_Print();
    return -1;
  }
  Fl::e_number = v;
  return 0;
}
PyObject *flpy_fl_get_int(PyObject *, void *v) {
  return Py_BuildValue("i", (int)(fl_intptr_t)v);
}
static PyGetSetDef flpy_fl_getset[] = {
  { "e_number", flpy_fl_get_e_number, flpy_fl_set_e_number, "Event number" },
  { "OPTION_ARROW_FOCUS", flpy_fl_get_int, NULL, NULL, (void*)0 },
  { "OPTION_VISIBLE_FOCUS", flpy_fl_get_int, NULL, NULL, (void*)1 },
  { NULL }
};

static PyTypeObject flpy_type_fl = {
  PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "fltk.Fl",
    .tp_doc = PyDoc_STR("Fl"),
//    .tp_basicsize = sizeof(Flpy_Object_Fl),
//    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT|Py_TPFLAGS_DISALLOW_INSTANTIATION, // Py_TPFLAGS_DISALLOW_INSTANTIATION, "Static Types"
//    .tp_new = PyType_GenericNew,
    .tp_methods = flpy_methods_fl,
    .tp_getset = flpy_fl_getset,
};

// ---- flpy_widget ------------------------------------------------------------

// When tp_dealloc is called, make sure that children are dereferenced

#define FlpyObject_HEAD \
  PyObject_HEAD \
  PyObject *callback_call; \
  PyObject *callback_data;

typedef struct {
  FlpyObject_HEAD
  Fl_Widget *o;
} Flpy_Object_Widget;

static void flpy_callback(Fl_Widget *w, void *v) {
  Flpy_Object_Widget *self = (Flpy_Object_Widget*)v;
  if (self->callback_call) {
    // Don't do that it it is already a tuple!
    // Actually, the first argument should be 'self', right? Or whatever do_callback() wants.
    PyObject *tuple = PyTuple_Pack(1, self->callback_data);
    Py_INCREF(tuple);
    PyObject *result = PyObject_CallObject(self->callback_call, tuple); //self->callback_data);
    Py_DECREF(tuple);
  }
}

static PyObject *flpy_method_widget_show(Flpy_Object_Widget *self, PyObject *args) {
  self->o->show();
  Py_RETURN_NONE;
}

static PyObject *flpy_method_widget_label(Flpy_Object_Widget *self, PyObject *args) {
  char *str = NULL;
  if (PyTuple_Size(args) == 0) {
    return PyUnicode_FromString(self->o->label());
  } else if (PyArg_ParseTuple(args, "z", &str)) {
    self->o->copy_label(str);
    Py_RETURN_NONE;
  }
  return NULL;
}

static PyObject *flpy_method_widget_callback(Flpy_Object_Widget *self, PyObject *args) {
  if (PyTuple_Size(args) == 0) {
    return self->callback_call;
  } else {
    self->callback_call = PyTuple_GetItem(args, 0);
    int v = PyCallable_Check(self->callback_call);
    Py_INCREF(self->callback_call);
    self->callback_data = PyTuple_GetItem(args, 1);
    Py_XINCREF(self->callback_data);
    Py_RETURN_NONE;
  }
  return NULL;
}

static PyMethodDef flpy_methods_widget[] = {
  { "show", (PyCFunction)flpy_method_widget_show, METH_NOARGS },
  { "label", (PyCFunction)flpy_method_widget_label, METH_VARARGS },
  { "callback", (PyCFunction)flpy_method_widget_callback, METH_VARARGS },
  { NULL }
};

// Fl_Widget should have no init method!?
static int flpy_widget_init(Flpy_Object_Widget *self, PyObject *args, PyObject*) {
  int x, y, w, h;
  if (!PyArg_ParseTuple(args, "iiii", &x, &y, &w, &h)) return NULL;
  Fl_Widget *o = self->o = new Fl_Box(x, y, w, h);
  o->callback(flpy_callback, self);
  if (o->parent()) Py_INCREF(self);
}

static PyTypeObject flpy_type_widget = {
  PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "fltk.Fl_Widget",
    .tp_doc = PyDoc_STR("Fl_Widget"),
    .tp_basicsize = sizeof(Flpy_Object_Widget),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, // how will we now if it is used to generate a new class?
    .tp_new = PyType_GenericNew,
    .tp_init = (initproc)flpy_widget_init,
    .tp_methods = flpy_methods_widget,
};

// ---- flpy_button ------------------------------------------------------------

typedef struct {
  FlpyObject_HEAD
  class Flpy_Button *o;
} Flpy_Object_Button;

class Flpy_Button : public Fl_Button {
public:
  Flpy_Button(int x, int y, int w, int h) : Fl_Button(x, y, w, h) { }
  void draw() {
    Flpy_Object_Button *self = (Flpy_Object_Button*)user_data();
    PyObject_CallMethodNoArgs((PyObject*)self, PyUnicode_FromString("draw"));
  }
};

static PyObject *flpy_method_button_value(Flpy_Object_Button *self, PyObject *args) {
  int v;
  if (PyTuple_Size(args) == 0) {
    return PyLong_FromLong(self->o->value());
  } else if (PyArg_ParseTuple(args, "i", &v)) {
    return PyLong_FromLong(self->o->value(v));
  }
  return NULL;
}

static PyObject *flpy_method_button_draw(Flpy_Object_Button *self, PyObject *args) {
  self->o->Fl_Button::draw();
  Py_RETURN_NONE;
}

static PyMethodDef flpy_methods_button[] = {
  { "value", (PyCFunction)flpy_method_button_value, METH_VARARGS },
  { "draw", (PyCFunction)flpy_method_button_draw, METH_NOARGS },
  { NULL }
};

//extern PyTypeObject flpy_type_button;
//static PyObject *flpy_button_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
//  Flpy_Object_Button *self = (Flpy_Object_Button*)PyType_GenericNew(type, args, kwds);
//  printf("%p %p\n", type, &flpy_type_button); // YES! This works!
//  return (PyObject*)self;
//}

static int flpy_button_init(Flpy_Object_Button *self, PyObject *args, PyObject*) {
  int x, y, w, h;
  char *label_;
  if (!PyArg_ParseTuple(args, "iiii|z", &x, &y, &w, &h, &label_)) return NULL;
  Flpy_Button *o = self->o = new Flpy_Button(x, y, w, h);
  // depending if this is just an instatiation, we can return just Fl_Button,
  // but if we derived a class, we should use Flpy_Button so users can override virtual methods?
  if (label_) o->copy_label(label_);
  o->callback(flpy_callback, self);
 if (o->parent()) Py_INCREF(self);
}

static PyTypeObject flpy_type_button = {
  PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "fltk.Fl_Button",
    .tp_doc = PyDoc_STR("Fl_Button"),
    .tp_basicsize = sizeof(Flpy_Object_Button),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, // how will we now if it is used to generate a new class?
//    .tp_new = flpy_button_new,
    .tp_new = [](PyTypeObject *type, PyObject *args, PyObject *kwds) -> PyObject* {
//      printf("%p %p\n", type, &flpy_type_button); // YES! This works! Also: PyTypeObject *Py_TYPE(PyObject *o)
      return PyType_GenericNew(type, args, kwds);
    },
    .tp_init = (initproc)flpy_button_init,
    .tp_methods = flpy_methods_button,
    .tp_base = &flpy_type_widget, // actually flpy_type_group...
};

// ---- flpy_window ------------------------------------------------------------

typedef struct {
  FlpyObject_HEAD
  Fl_Window *o;
} Flpy_Object_Window;

static PyObject *flpy_method_window_show(Flpy_Object_Window *self, PyObject *args) {
  self->o->show(0, NULL);
  Py_RETURN_NONE;
}

static PyMethodDef flpy_methods_window[] = {
  { "show", (PyCFunction)flpy_method_window_show, METH_VARARGS },
  { NULL }
};

static int flpy_window_init(Flpy_Object_Window *self, PyObject *args, PyObject*) {
  int w, h;
  char *label_;
  if (!PyArg_ParseTuple(args, "ii|z", &w, &h, &label_)) return NULL;
  Fl_Window *o = self->o = new Fl_Window(w, h);
  if (label_) o->copy_label(label_);
  o->user_data(self);
  if (o->parent()) Py_INCREF(self);
}

static PyTypeObject flpy_type_window = {
  PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "fltk.Fl_Window",
    .tp_doc = PyDoc_STR("Fl_Window"),
    .tp_basicsize = sizeof(Flpy_Object_Window),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, // how will we now if it is used to generate a new class?
    .tp_new = PyType_GenericNew,
    .tp_init = (initproc)flpy_window_init,
    .tp_methods = flpy_methods_window,
    .tp_base = &flpy_type_widget, // actually flpy_type_group...
};

// ---- flpy_window ------------------------------------------------------------

PyMODINIT_FUNC
PyInit_fltk(void)
{
//  if (PyType_Ready(&flpyo_widget) < 0)
//    return NULL;

  PyObject *m = PyModule_Create(&fltk_module);

  PyType_Ready(&flpy_type_fl);
//  Py_INCREF(&flpy_type_fl);
//  PyModule_AddObjectRef(m, "_Fl", (PyObject *)&flpy_type_fl);
//  PyModule_AddObjectRef(<#PyObject *mod#>, <#const char *name#>, <#PyObject *value#>)
  PyObject *FlPy = PyObject_New(PyObject, &flpy_type_fl);
  PyModule_AddObjectRef(m, "Fl", FlPy);

  PyType_Ready(&flpy_type_widget);
  Py_INCREF(&flpy_type_widget);
  PyModule_AddObjectRef(m, "Fl_Widget", (PyObject *)&flpy_type_widget);

  PyType_Ready(&flpy_type_button);
  Py_INCREF(&flpy_type_button);
  PyModule_AddObjectRef(m, "Fl_Button", (PyObject *)&flpy_type_button);

  PyType_Ready(&flpy_type_window);
  Py_INCREF(&flpy_type_window);
  PyModule_AddObjectRef(m, "Fl_Window", (PyObject *)&flpy_type_window);

//  Py_INCREF(&flpyo_widget);
//  if (PyModule_AddObject(m, "Fl_Widget", (PyObject *)&flpyo_widget) < 0) {
//    Py_DECREF(&flpyo_widget);
//    Py_DECREF(m);
//    return NULL;
//  }
//
//  PyModule_AddIntMacro(m, FL_RED);
//
  return m;
}



int main(int argc, char **argv) {
  PyImport_AppendInittab("fltk", PyInit_fltk);
  Py_Initialize();
  PyRun_SimpleString(prg);
  Py_Finalize();

//  Fl_Window *window = new Fl_Window(340, 180);
//  Fl_Box *box = new Fl_Box(20, 40, 300, 100, "Hello, World!");
//  box->box(FL_UP_BOX);
//  box->labelfont(FL_BOLD + FL_ITALIC);
//  box->labelsize(36);
//  box->labeltype(FL_SHADOW_LABEL);
//  window->end();
//  window->show(argc, argv);
//  return Fl::run();
}
