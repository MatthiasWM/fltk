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

#include "application/history.h"
#include "fluid.h"

using namespace fluid;

// File history info...
char application::History::full_path[10][FL_PATH_MAX];
char application::History::short_path[10][FL_PATH_MAX];

/**
 Load project file history from preferences.

 This C++ function, fluid::App_History::load(), loads the last 10 used 
 .fl project files' absolute and relative paths from the application 
 preferences and updates the main menu accordingly. It ensures the 
 history list is limited to 10 files and handles empty or 
 non-existent file paths. 
 */
void application::History::load() {
  int   i;              // Looping var
  int   max_files;


  fluid_prefs.get("recent_files", max_files, 5);
  if (max_files > 10) max_files = 10;

  for (i = 0; i < max_files; i ++) {
    fluid_prefs.get( Fl_Preferences::Name("file%d", i), full_path[i], "", sizeof(full_path[i]));
    if (full_path[i][0]) {
      // Make a relative version of the filename for the menu...
      fl_filename_relative(short_path[i], sizeof(short_path[i]),
                           full_path[i]);

      if (i == 9) history_item[i].flags = FL_MENU_DIVIDER;
      else history_item[i].flags = 0;
    } else break;
  }

  for (; i < 10; i ++) {
    if (i) history_item[i-1].flags |= FL_MENU_DIVIDER;
    history_item[i].hide();
  }
}

/**
 Add a file to the project file history.

 This C++ function adds a new file path to the application's file history, 
 which is stored in the application's preferences.

 The function ensures that the history is limited to a maximum of 10 files,
 and it handles the case where the new file is already in the history to
 prevent duplicates.

 \param[in] flname path or filename of .fl file, will be converted into an
    absolute file path based on the current working directory.
 */
void application::History::add(const char *flname) {
  int   i;              // Looping var
  char  absolute[FL_PATH_MAX];
  int   max_files;


  fluid_prefs.get("recent_files", max_files, 5);
  if (max_files > 10) max_files = 10;

  fl_filename_absolute(absolute, sizeof(absolute), flname);

  for (i = 0; i < max_files; i ++)
#if defined(_WIN32) || defined(__APPLE__)
    if (!strcasecmp(absolute, full_path[i])) break;
#else
    if (!strcmp(absolute, full_path[i])) break;
#endif // _WIN32 || __APPLE__

  if (i == 0) return;

  if (i >= max_files) i = max_files - 1;

  // Move the other flnames down in the list...
  memmove(full_path + 1, full_path,
          i * sizeof(full_path[0]));
  memmove(short_path + 1, short_path,
          i * sizeof(short_path[0]));

  // Put the new file at the top...
  strlcpy(full_path[0], absolute, sizeof(full_path[0]));

  fl_filename_relative(short_path[0], sizeof(short_path[0]),
                       full_path[0]);

  // Update the menu items as needed...
  for (i = 0; i < max_files; i ++) {
    fluid_prefs.set( Fl_Preferences::Name("file%d", i), full_path[i]);
    if (full_path[i][0]) {
      if (i == 9) history_item[i].flags = FL_MENU_DIVIDER;
      else history_item[i].flags = 0;
    } else break;
  }

  for (; i < 10; i ++) {
    fluid_prefs.set( Fl_Preferences::Name("file%d", i), "");
    if (i) history_item[i-1].flags |= FL_MENU_DIVIDER;
    history_item[i].hide();
  }
  fluid_prefs.flush();
}
