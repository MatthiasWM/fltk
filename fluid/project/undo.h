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

#ifndef FLUID_PROJECT_UNDO_H
#define FLUID_PROJECT_UNDO_H

class Fl_Widget;

namespace fluid {

class Project;

namespace project {

class Undo {
  int paused { 0 };             // Undo checkpointing paused?
  int max_undo { 0 }; // Maximum undo level used
public:

  static constexpr int kUndoWindowResize { 1 };

  Project &project;

  int current { 0 };                // Current undo level in buffer
  int last { 0 };                   // Last undo level in buffer
  int last_saved { -1 };                   // Last undo level that was saved
  int once_type;              // Suspend further undos of the same type

  Undo(Project &proj);
  void undo();
  void redo();
  void checkpoint();                 // Save current file to undo buffer
  void checkpoint_once(int type);    // Save undo buffer once until a different checkpoint type is called
  void clear();                      // Clear undo buffer
  void suspend();                    // Suspend undo checkpoints
  void resume();                     // Resume undo checkpoints
};

}

}

#endif // FLUID_PROJECT_UNDO_H
