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

namespace fluid {

class Application;
class Project;

class Callbacks {
public:
  static void save(Fl_Widget *, void *v);
};

}; // namespace FLUID

extern fluid::Project g_project;
extern fluid::Application Fluid;

// ---- public functions

extern void set_modflag(int mf, int mfc=-1);

extern Fl_String end_with_slash(const Fl_String &str);
extern void flush_text_widgets();

// ---- public callback functions

extern void save_template_cb(Fl_Widget *, void *);
extern void revert_cb(Fl_Widget *,void *);
extern void exit_cb(Fl_Widget *,void *);

extern void write_strings_cb(Fl_Widget *, void *);
extern void align_widget_cb(Fl_Widget *, long);
extern void toggle_widgetbin_cb(Fl_Widget *, void *);

extern char position_window(Fl_Window *w, const char *prefsName, int Visible, int X, int Y, int W=0, int H=0);

#endif // _FLUID_FLUID_H
