//
// Project Class for Fast Light User Interface Designer (FLUID).
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

#ifndef FLUID_PROJECT_PROJECT_H
#define FLUID_PROJECT_PROJECT_H

#include "../src/Fl_String.H"
#include "fluid.h"

namespace fluid {

class Application;

/**
 Data and settings for a FLUID project file.
 */
class Project {
public:
  /// Project constructor
  Project() = default;
  /// Project destructor - not implemented
  ~Project() = default;
  void reset();
  void update_settings_dialog();
  void save(bool ask_for_filename, bool update_filename);
  int write_code_files(bool dont_show_completion_dialog=false);
  /// Give the user the opportunity to save a project before clearing it.
  bool confirm_clear();

  /// Change the current working directory to the .fl project directory.
  void enter_project_dir();
  /// Change the current working directory to the previous directory.
  void leave_project_dir();

  Fl_String projectfile_path() const;
  Fl_String projectfile_name() const;
  Fl_String codefile_path() const;
  Fl_String codefile_name() const;
  Fl_String headerfile_path() const;
  Fl_String headerfile_name() const;
  Fl_String stringsfile_path() const;
  Fl_String stringsfile_name() const;
  Fl_String basename() const;

  /// One of the available internationalization types.
  Fd_I18n_Type i18n_type { Fd_I18n_Type::NONE };
  /// Include file for GNU i18n, writes an #include statement into the source
  /// file. This is usually `<libintl.h>` or `"gettext.h"` for GNU gettext.
  Fl_String i18n_gnu_include { "<libintl.h>" };
  // Optional name of a macro for conditional i18n compilation.
  Fl_String i18n_gnu_conditional { };
  /// For the gettext/intl.h options, this is the function that translates text
  /// at runtime. This is usually "gettext" or "_".
  Fl_String i18n_gnu_function { "gettext" };
  /// For the gettext/intl.h options, this is the function that marks the translation
  /// of text at initialisation time. This is usually "gettext_noop" or "N_".
  Fl_String i18n_gnu_static_function { "gettext_noop" };

  /// Include file for Posix i18n, write a #include statement into the source
  /// file. This is usually `<nl_types.h>` for Posix catgets.
  Fl_String i18n_pos_include { "<nl_types.h>" };
  // Optional name of a macro for conditional i18n compilation.
  Fl_String i18n_pos_conditional { };
  /// Name of the nl_catd database
  Fl_String i18n_pos_file { };
  /// Message set ID for the catalog.
  Fl_String i18n_pos_set { "1" };

  /// If set, generate code to include the header file form the c++ file
  int include_H_from_C { 1 };
  /// If set, handle keyboard shortcut Ctrl on macOS using Cmd instead
  int use_FL_COMMAND { 0 };
  /// Clear if UTF-8 characters in statics texts are written as escape sequences
  int utf8_in_src { 0 };
  /// If set, <FL/Fl.H> will not be included from the header code before anything else
  int avoid_early_includes { 0 };
  /// If set, command line overrides header file name in .fl file.
  int header_file_set { 0 };
  ///  If set, command line overrides source code file name in .fl file.
  int code_file_set { 0 };
  int write_mergeback_data { 0 };
  /// Hold the default extension for header files, or the entire filename if set via command line.
  Fl_String header_file_name { ".h" };
  /// Hold the default extension for source code  files, or the entire filename if set via command line.
  Fl_String code_file_name { ".cxx" };
  /// Filename of the current .fl project file
  const char *filename { nullptr };
  /// Used as a counter to set the .fl project dir as the current directory.
  char in_project_dir { 0 };
};

} // namespace fluid

#endif // FLUID_PROJECT_PROJECT_H
