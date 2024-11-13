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
#include "application/args.h"
#include "application/history.h"
#include "application/settings.h"

#include <string>

namespace fluid {

class Application {
  /// path to store temporary files during app run
  Fl_String tmpdir_path { };
  /// true if the temporary file path was already created
  bool tmpdir_create_called { false };
public:
  /// Command line arguments.
  application::Args args { *this };
  /// Application settings.
  application::Settings settings;
  /// Application history.
  application::History history;
  /// Set if the application is not in interactive mode.
  bool batch_mode { false };
  /// current directory path at application launch
  Fl_String launch_path { };

  /// Clear the current project and create a new, empty one.
  bool new_project(bool user_must_confirm = true);
  /// Get the current project.
  Project &project();

  /// Generate a path to a directory for temporary data storage.
  void create_tmpdir();
  /// Delete the temporary directory that was created in set_tmpdir.
  void delete_tmpdir();
  /// Return the path to a temporary directory for this instance of FLUID.
  const Fl_String &get_tmpdir();
  /// Return the path and filename of a temporary file for cut or duplicated data.
  static char* cutfname(int which = 0);
};

}; // namespace FLUID

#endif // FLUID_APPLICATION_APPLICATION_H
