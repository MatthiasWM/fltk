//
// Application Args Class for Fast Light User Interface Designer (FLUID).
//
// Copyright 1998-2024 by Bill Spitzak and others.
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

#ifndef FLUID_APPLICATION_ARGS_H
#define FLUID_APPLICATION_ARGS_H

#include "../src/Fl_String.H"

namespace fluid {

class Application;

namespace application {
        
class Args {
private:
  Application &app;
public:
  /// Argsconstructor
  Args(Application &theApp);
  /// Read command line args.
  int read(int argc, char **argv);
  /// Read one command line argument
  int arg(int argc, char** argv, int& i);
  /// FLTK callback to ead one command line argument
  static int arg_callback(int argc, char** argv, int& i);
  /// `-o filename`: override the generate code file extension or name
  Fl_String code_filename { };
  /// `-h filename`: override the generate header file extension or name
  Fl_String header_filename { };
  /// `--autodoc path`: if set, generate images for automatic documentation in this directory
  Fl_String autodoc_path { };
  /// `-u`: update the project file
  bool update { false };
  /// `-c`: compile the project file into source code
  bool compile { false };
  /// `-cs`: compile the project file into source code and write i18n strings file
  bool strings { false };
  /// `-d`: debug mode (used by external code editor)
  bool debug { false };
};

} // namespace application

} // namespace fluid

#endif // FLUID_APPLICATION_ARGS_H
