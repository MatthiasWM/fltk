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
"\n"
"def test_callback(a):\n"
"  print('Hallo Welt')\n"
"  print(a)\n"
"  print(FL_BOLD|FL_ITALIC)\n"
"  print(btn.flags())\n"
"\n"
"class MyButton(Fl_Button):\n"
"  def draw(self):\n"
"    fl_color(3)\n"
"    fl_rectf(10, 10, 150, 100)\n"
"    fl_color(4)\n"
"    fl_rectf(40, 40, 150, 100)\n"
"    super().draw()\n"
"\n"
"# if __name__ == '__main__':\n"
"#   main()\n"
"\n"
"window = Fl_Window(340, 180, 'Hello Python')\n"
"#btn = MyButton(20, 40, 300, 100, 'Hello, World!')\n"
"btn = Fl_Button(20, 40, 300, 100, 'Hello, World!')\n"
"#btn = Fl_Button(20, 40, 300, 100)\n"
"btn.callback(test_callback, 'Callback called')\n"
"#  box->box(FL_UP_BOX);\n"
"#  box->labelfont(FL_BOLD + FL_ITALIC);\n"
"#  box->labelsize(36);\n"
"#  box->labeltype(FL_SHADOW_LABEL);\n"
"#  window->end();\n"
"#  window->show(argc, argv);\n"
"#window.label('Hello!', 123)\n"
"window.show()\n"
"#  return Fl::run();\n"
"Fl.run()\n"
;


int main(int argc, char **argv) {
  PyImport_AppendInittab("fltk", PyInit_fltk);
  Py_Initialize();
  PyRun_SimpleString(prg);
  Py_Finalize();
}

