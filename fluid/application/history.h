//
// Application Histroy Class for Fast Light User Interface Designer (FLUID).
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

#ifndef FLUID_APPLICATION_HISTORY_H
#define FLUID_APPLICATION_HISTORY_H

#include <FL/filename.H>

namespace fluid {

namespace application {
        
class History {
public:
  /// Stores the absolute filename of the last 10 design files, saved in app preferences.
  static char full_path[10][FL_PATH_MAX];
  /// This list of filenames is computed from \c full_path and displayed in the main menu.
  static char short_path[10][FL_PATH_MAX];
  /// \brief Load project file history from preferences.
  void load();
  /// \brief Add a file to the project file history.
  void add(const char *);
};

} // namespace application

} // namespace fluid

#endif // FLUID_APPLICATION_HISTORY_H
