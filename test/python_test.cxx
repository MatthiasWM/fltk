//
// Python test program for the Fast Light Tool Kit (FLTK).
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

//find_package(PythonLibs REQUIRED)
//include_directories(${PYTHON_INCLUDE_DIRS})
//target_link_libraries(<your exe or lib> ${PYTHON_LIBRARIES})
//For details of the commands, run:
//
//cmake --help-module FindPythonLibs
//cmake --help-command find_package
//cmake --help-command include_directories
//cmake --help-command target_link_libraries

// TODO: if an exception occured in the callback, don't return to the main event loop!

#define PY_SSIZE_T_CLEAN
#include <Python.h>

extern "C" PyObject *PyInit_fltk();

#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>

const char *prg =

"from fltk import *\n"
"import sys, gc\n"
"\n"
"def test_callback(w, a):\n"
"  print('test callback called:', a)\n"
"  fl_clear_current()\n"
"  z1 = Fl_Button(10, 10, 100, 100)\n"
"  z2 = Fl_Widget(10, 10, 100, 100)\n"
"  gc.collect()\n"
"\n"
"class MyButton(Fl_Button):\n"
"#  def __init__(self, x, y, w, h, lab):\n"
"#    self.callback(None)\n"
"  def draw(self):\n"
"    fl_color(3)\n"
"    fl_rectf(10, 10, 150, 100)\n"
"    fl_color(4)\n"
"    fl_rectf(40, 40, 150, 100)\n"
"    super().draw()\n"
"  def on_callback(self, a):\n"
"    print('on_callback callback called:', a)\n"
"\n"
"window = Fl_Window(340, 180, 'Hello Python')\n"
"#btn = MyButton(20, 40, 300, 100, 'Hello, World!')\n"
"btn = Fl_Button(20, 40, 300, 100, 'Hello, World!')\n"
"Fl_Button(20, 140, 300, 20, 'Hello, World!').color(FL_RED)\n"
"#btn = Fl_Button(20, 40, 300, 100)\n"
"#btn.callback(test_callback, 'Callback called')\n"
"#btn.toast = 'yum'\n"
"#  box->box(FL_UP_BOX);\n"
"#  box->labelfont(FL_BOLD + FL_ITALIC);\n"
"#  box->labelsize(36);\n"
"#  box->labeltype(FL_SHADOW_LABEL);\n"
"#  window->end();\n"
"#  window->show(argc, argv);\n"
"#print( window.measure_label(120) )\n"
"#window.show(sys.argv)\n"
"window.show()\n"
"#print( sys.getrefcount(btn) )\n"
"#print( gc.get_referrers(window) )\n"
"#print( gc.get_referents(window) )\n"
"#  return Fl::run();\n"
"Fl.run()\n"
;


int main(int argc, char **argv) {
  printf("Compiled with v%d.%d.%d (runtime %08lx)\n", PY_MAJOR_VERSION, PY_MINOR_VERSION, PY_MICRO_VERSION, Py_Version);
  PyImport_AppendInittab("fltk", PyInit_fltk);
  Py_Initialize();
  PyRun_SimpleString(prg);
  Py_Finalize();
}

