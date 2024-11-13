//
// FLUID main entry for the Fast Light Tool Kit (FLTK).
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

#ifndef _FLUID_FLUID_H
#define _FLUID_FLUID_H

#include "fluid_filename.h"
#include <FL/Fl_Preferences.H>
#include <FL/Fl_Menu_Item.H>
#include "../src/Fl_String.H"

#include <string>

constexpr int BROWSERWIDTH { 300 };
constexpr int BROWSERHEIGHT { 500 };
constexpr int WINWIDTH { 300 };
constexpr int MENUHEIGHT { 25 };
constexpr int WINHEIGHT { BROWSERHEIGHT+MENUHEIGHT };

// ---- types

class Fl_Double_Window;
class Fl_Window;
class Fl_Menu_Bar;
class Fl_Type;
class Fl_Choice;
class Fl_Button;
class Fl_Check_Button;

/**
 Indicate the storage location for tools like layout suites and shell macros.
 \see class Fd_Shell_Command, class Fd_Layout_Suite
 */
enum class Fd_Tool_Store {
  INTERNAL,  ///< stored inside FLUID app
  USER,      ///< suite is stored in the user wide FLUID settings
  PROJECT,   ///< suite is stored within the current .fl project file
  FILE       ///< store suite in external file
};

// ---- global variables

extern Fl_Preferences fluid_prefs;
extern Fl_Menu_Item Main_Menu[];
extern Fl_Menu_Bar *main_menubar;
extern Fl_Window *main_window;

extern int reading_file;

extern Fl_Menu_Item *save_item;
extern Fl_Menu_Item *history_item;
extern Fl_Menu_Item *widgetbin_item;
extern Fl_Menu_Item *codeview_item;
extern Fl_Menu_Item *overlay_item;
extern Fl_Button *overlay_button;
extern Fl_Menu_Item *guides_item;
extern Fl_Menu_Item *restricted_item;
extern Fl_Check_Button *guides_button;

extern int modflag;

extern int pasteoffset;

// ---- project class declaration

/**
 Enumeration of available internationalization types.
 */
enum class Fd_I18n_Type {
  NONE = 0, ///< No i18n, all strings are litearals
  GNU,      ///< GNU gettext internationalization
  POSIX     ///< Posix catgets internationalization
};

namespace FLUID {

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
};

class App_Args {
public:
  /// Read command line args.
  bool read(int argc, char **argv);
  /// Read one command line argument
  static int arg(int argc, char** argv, int& i);
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

class App_Settings {
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

class App_History {
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

class Callbacks {
public:
  static void save(Fl_Widget *, void *v);
};

class App {
public:
  /// Command line arguments.
  App_Args args;
  /// Application settings.
  App_Settings settings;
  /// Application history.
  App_History history;
  /// Set if the application is running from the command line and not in interactive mode.
  bool batch_mode { false };
  /// current directory path at application launch
  Fl_String launch_path { };

  /// Clear the current project and create a new, empty one.
  bool new_project(bool user_must_confirm = true);
  /// Get the current project.
  Project &project();
};


}; // namespace FLUID

extern FLUID::Project g_project;
extern FLUID::App Fluid;

// ---- public functions

extern void enter_project_dir();
extern void leave_project_dir();
extern void set_filename(const char *c);
extern void set_modflag(int mf, int mfc=-1);

extern const Fl_String &get_tmpdir();

// ---- public callback functions

extern void save_template_cb(Fl_Widget *, void *);
extern void revert_cb(Fl_Widget *,void *);
extern void exit_cb(Fl_Widget *,void *);

extern void write_strings_cb(Fl_Widget *, void *);
extern void align_widget_cb(Fl_Widget *, long);
extern void toggle_widgetbin_cb(Fl_Widget *, void *);

extern char position_window(Fl_Window *w, const char *prefsName, int Visible, int X, int Y, int W=0, int H=0);

#endif // _FLUID_FLUID_H
