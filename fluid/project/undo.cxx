//
// Project Undo Class for Fast Light User Interface Designer (FLUID).
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

#include "project/undo.h"

#include "fluid.h"
#include "application/application.h"
#include "project/project.h"
#include "file.h"
#include "Fl_Type.h"
#include "Fl_Widget_Type.h"
#include "widget_browser.h"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Preferences.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/fl_ask.H>
#include "fluid_filename.h"
#include "../src/flstring.h"

#if defined(_WIN32) && !defined(__CYGWIN__)
#  include <io.h>
#  include <windows.h>
#  define getpid (int)GetCurrentProcessId
#else
#  include <unistd.h>
#endif // _WIN32 && !__CYGWIN__


//
// This file implements an undo system using temporary files; ideally
// we'd like to do this in memory, however the current data structures
// and design aren't well-suited...  Instead, we save and restore
// checkpoint files.
//

extern Fl_Window* the_panel;

using namespace fluid;


// Return the undo filename.
// The filename is constructed in a static internal buffer and
// this buffer is overwritten by every call of this function.
// The return value is a pointer to this internal string.
// NOTE: the filename should be defined per project and contain the project 
// NOTE: filename or another unique ID.
static char *undo_filename(int level) {
  static char undo_path[FL_PATH_MAX] = ""; // Undo path
  static unsigned int undo_path_len = 0;   // length w/o filename

  if (!undo_path_len) {
    fluid_prefs.getUserdataPath(undo_path, sizeof(undo_path));
    undo_path_len = (unsigned int)strlen(undo_path);
  }

  // append filename: "undo_PID_LEVEL.fl"
  snprintf(undo_path + undo_path_len,
           sizeof(undo_path) - undo_path_len - 1,
           "undo_%d_%d.fl", getpid(), level);
  return undo_path;
}

project::Undo::Undo(Project &proj)
: project(proj)
{
}

void project::Undo::redo() {
  // int undo_item = main_menubar->find_index(undo_cb);
  // int redo_item = main_menubar->find_index(redo_cb);
  once_type = 0;

  if (current >= last) {
    fl_beep();
    return;
  }

  project.undo.suspend();
  if (widget_browser) widget_browser->save_scroll_position();
  int reload_panel = (the_panel && the_panel->visible());
  if (!read_file(undo_filename(current + 1), 0)) {
    // Unable to read checkpoint file, don't redo...
    widget_browser->rebuild();
    project.update_settings_dialog();
    project.undo.resume();
    return;
  }
  if (reload_panel) {
    for (Fl_Type *t = Fl_Type::first; t; t=t->next) {
      if (t->is_widget() && t->selected)
        t->open();
    }
  }
  if (widget_browser) widget_browser->restore_scroll_position();

  current ++;

  // Update modified flag...
  project.set_modflag(current != last_saved);
  widget_browser->rebuild();
  project.update_settings_dialog();

  // Update undo/redo menu items...
  // if (current >= last) Main_Menu[redo_item].deactivate();
  // Main_Menu[undo_item].activate();
}


void project::Undo::undo() {
  // int undo_item = main_menubar->find_index(undo_cb);
  // int redo_item = main_menubar->find_index(redo_cb);
  once_type = 0;

  if (current <= 0) {
    fl_beep();
    return;
  }

  if (current == last) {
    write_file(undo_filename(current));
  }

  project.undo.suspend();
  // Undo first deletes all widgets which resets the widget_tree browser.
  // Save the current scroll position, so we don't scroll back to 0 at undo.
  // TODO: make the scroll position part of the .fl project file
  if (widget_browser) widget_browser->save_scroll_position();
  int reload_panel = (the_panel && the_panel->visible());
  if (!read_file(undo_filename(current - 1), 0)) {
    // Unable to read checkpoint file, don't undo...
    widget_browser->rebuild();
    project.update_settings_dialog();
    project.set_modflag(0, 0);
    project.undo.resume();
    return;
  }
  if (reload_panel) {
    for (Fl_Type *t = Fl_Type::first; t; t=t->next) {
      if (t->is_widget() && t->selected) {
        t->open();
        break;
      }
    }
  }
  // Restore old browser position.
  // Ideally, we would save the browser position insied the undo file.
  if (widget_browser) widget_browser->restore_scroll_position();

  current --;

  // Update modified flag...
  project.set_modflag(current != last_saved);

  // Update undo/redo menu items...
  // if (current <= 0) Main_Menu[undo_item].deactivate();
  // Main_Menu[redo_item].activate();
  widget_browser->rebuild();
  project.update_settings_dialog();
  project.undo.resume();
}

void project::Undo::checkpoint_once(int type) {
  if (paused) return;
  if (once_type != type) {
    project.undo.checkpoint();
    once_type = type;
  } else {
    // do not add more checkpoints for the same undo typw
  }
}

// Save current file to undo buffer
void project::Undo::checkpoint() {
  //  printf("undo::checkpoint(): current=%d, paused=%d, modflag=%d\n",
  //         current, paused, modflag);

  // Don't checkpoint if project.undo.suspend() has been called...
  if (paused) return;

  // int undo_item = main_menubar->find_index(undo_cb);
  // int redo_item = main_menubar->find_index(redo_cb);
  once_type = 0;

  // Save the current UI to a checkpoint file...
  const char *filename = undo_filename(current);
  if (!write_file(filename)) {
    // Don't attempt to do undo stuff if we can't write a checkpoint file...
    perror(filename);
    return;
  }

  // Update the saved level...
  if (project.modflag && current <= last_saved) last_saved = -1;
  else if (!project.modflag) last_saved = current;

  // Update the current undo level...
  current ++;
  last = current;
  if (current > max_undo) max_undo = current;

  // Enable the Undo and disable the Redo menu items...
  // Main_Menu[undo_item].activate();
  // Main_Menu[redo_item].deactivate();
}

// Clear undo buffer
void project::Undo::clear() {
  // int undo_item = main_menubar->find_index(undo_cb);
  // int redo_item = main_menubar->find_index(redo_cb);
  // Remove old checkpoint files...
  for (int i = 0; i <= max_undo; i ++) {
    fl_unlink(undo_filename(i));
  }

  // Reset current, last, and save indices...
  current = last = max_undo = 0;
  if (project.modflag) last_saved = -1;
  else last_saved = 0;

  // Disable the Undo and Redo menu items...
  // Main_Menu[undo_item].deactivate();
  // Main_Menu[redo_item].deactivate();
}

// Resume undo checkpoints
void project::Undo::resume() {
  paused--;
}

// Suspend undo checkpoints
void project::Undo::suspend() {
  paused++;
}
