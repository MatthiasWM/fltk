//
// Application Class for Fast Light User Interface Designer (FLUID).
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

#ifndef FLUID_APPLICATION_APPLICATION_H
#define FLUID_APPLICATION_APPLICATION_H

#include "fluid.h"

#include <string>

namespace fluid {

class Application {
public:
  /// Command line arguments.
  App_Args args;
  /// Application settings.
  App_Settings settings;
  /// Application history.
  App_History history;
  /// Set if the application is not in interactive mode.
  bool batch_mode { false };
  /// current directory path at application launch
  Fl_String launch_path { };

  /// Clear the current project and create a new, empty one.
  bool new_project(bool user_must_confirm = true);
  /// Get the current project.
  Project &project();
};

}; // namespace FLUID

#endif // FLUID_APPLICATION_APPLICATION_H
