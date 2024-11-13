//
// Application Settings Class for Fast Light User Interface Designer (FLUID).
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

#ifndef FLUID_APPLICATION_SETTINGS_H
#define FLUID_APPLICATION_SETTINGS_H

#include <string>

namespace fluid {

namespace application {
        
class Settings {
public:
  /// Show guides in the design window when positioning widgets, saved in app preferences.
  int show_guides { 1 };
  /// Show areas of restricted use in overlay plane.
  /// Restricted areas are widget that overlap each other, widgets that are outside
  /// of their parent's bounds (except children of Scroll groups), and areas
  /// within an Fl_Tile that are not covered by children.
  int show_restricted { 1 };
  /// Show a ghosted outline for groups that have very little contrast.
  /// This makes groups with NO_BOX or FLAT_BOX better editable.
  int show_ghosted_outline { 1 };
  /// Show widget comments in the browser, saved in app preferences.
  int show_comments { 1 };
  /// Use external editor for editing Fl_Code_Type, saved in app preferences.
  int use_external_editor { 0 };
  /// Run this command to load an Fl_Code_Type into an external editor, save in app preferences.
  std::string external_editor_command { };
};

} // namespace application

} // namespace fluid

#endif // FLUID_APPLICATION_SETTINGS_H
