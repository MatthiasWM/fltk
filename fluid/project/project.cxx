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

#include "project/project.h"
#include "application/application.h"
#include "fluid.h"
#include "file.h"
#include "undo.h"
#include "Fl_Type.h"
#include "settings_panel.h"

using namespace fluid;


/// The current project, possibly a new, empty roject
fluid::Project g_project;

/**
 Reset all project setting to create a new empty project.
 */
void Project::reset() {
  ::delete_all();
  i18n_type = Fd_I18n_Type::NONE;

  i18n_gnu_include = "<libintl.h>";
  i18n_gnu_conditional = "";
  i18n_gnu_function = "gettext";
  i18n_gnu_static_function = "gettext_noop";

  i18n_pos_include = "<nl_types.h>";
  i18n_pos_conditional = "";
  i18n_pos_file = "";
  i18n_pos_set = "1";

  include_H_from_C = 1;
  use_FL_COMMAND = 0;
  utf8_in_src = 0;
  avoid_early_includes = 0;
  header_file_set = 0;
  code_file_set = 0;
  header_file_name = ".h";
  code_file_name = ".cxx";
  write_mergeback_data = 0;
}

/**
 Tell the project and i18n tab of the settings dialog to refresh themselves.
 */
void Project::update_settings_dialog() {
  if (settings_window) {
    w_settings_project_tab->do_callback(w_settings_project_tab, LOAD);
    w_settings_i18n_tab->do_callback(w_settings_i18n_tab, LOAD);
  }
}

/**
 Give the user the opportunity to save a project before clearing it.

 If the project has unsaved changes, this function pops up a dialog, that
 allows the user to save the project, continue without saving the project,
 or to cancel the operation.

 If the user chooses to save, and no filename was set, a file dialog allows
 the user to pick a name and location, or to cancel the operation.

 \return false if the user aborted the operation and the calling function
 should abort as well
 */
bool Project::confirm_clear() {
  if (modflag == 0) return true;
  switch (fl_choice("This project has unsaved changes. Do you want to save\n"
                    "the project file before proceeding?",
                    "Cancel", "Save", "Don't Save"))
  {
    case 0 : /* Cancel */
      return false;
    case 1 : /* Save */
      fluid::Callbacks::save(NULL, NULL);
      if (modflag) return false;  // user canceled the "Save As" dialog
  }
  return true;
}

/**
 Get the absolute path of the project file, for example `/Users/matt/dev/`.
 \return the path ending in '/'
 */
Fl_String Project::projectfile_path() const {
  return end_with_slash(fl_filename_absolute(fl_filename_path(filename), Fluid.launch_path));
}

/**
 Get the project file name including extension, for example `test.fl`.
 \return the file name without path
 */
Fl_String Project::projectfile_name() const {
  return fl_filename_name(filename);
}

/**
 Get the absolute path of the generated C++ code file, for example `/Users/matt/dev/src/`.
 \return the path ending in '/'
 */
Fl_String Project::codefile_path() const {
  Fl_String path = fl_filename_path(code_file_name);
  if (Fluid.batch_mode)
    return end_with_slash(fl_filename_absolute(path, Fluid.launch_path));
  else
    return end_with_slash(fl_filename_absolute(path, projectfile_path()));
}

/**
 Get the generated C++ code file name including extension, for example `test.cxx`.
 \return the file name without path
 */
Fl_String Project::codefile_name() const {
  Fl_String name = fl_filename_name(code_file_name);
  if (name.empty()) {
    return fl_filename_setext(fl_filename_name(filename), ".cxx");
  } else if (name[0] == '.') {
    return fl_filename_setext(fl_filename_name(filename), code_file_name);
  } else {
    return name;
  }
}

/**
 Get the absolute path of the generated C++ header file, for example `/Users/matt/dev/src/`.
 \return the path ending in '/'
 */
Fl_String Project::headerfile_path() const {
  Fl_String path = fl_filename_path(header_file_name);
  if (Fluid.batch_mode)
    return end_with_slash(fl_filename_absolute(path, Fluid.launch_path));
  else
    return end_with_slash(fl_filename_absolute(path, projectfile_path()));
}

/**
 Get the generated C++ header file name including extension, for example `test.cxx`.
 \return the file name without path
 */
Fl_String Project::headerfile_name() const {
  Fl_String name = fl_filename_name(header_file_name);
  if (name.empty()) {
    return fl_filename_setext(fl_filename_name(filename), ".h");
  } else if (name[0] == '.') {
    return fl_filename_setext(fl_filename_name(filename), header_file_name);
  } else {
    return name;
  }
}

/**
 Get the absolute path of the generated i18n strings file, for example `/Users/matt/dev/`.
 Although it may be more useful to put the text file into the same directory
 with the source and header file, historically, the text is always saved with
 the project file in interactive mode, and in the FLUID launch directory in
 batch mode.
 \return the path ending in '/'
 */
Fl_String Project::stringsfile_path() const {
  if (Fluid.batch_mode)
    return Fluid.launch_path;
  else
    return projectfile_path();
}

/**
 Get the generated i18n text file name including extension, for example `test.po`.
 \return the file name without path
 */
Fl_String Project::stringsfile_name() const {
  switch (i18n_type) {
    default: return fl_filename_setext(fl_filename_name(filename), ".txt");
    case Fd_I18n_Type::GNU: return fl_filename_setext(fl_filename_name(filename), ".po");
    case Fd_I18n_Type::POSIX: return fl_filename_setext(fl_filename_name(filename), ".msg");
  }
}

/**
 Get the name of the project file without the filename extension.
 \return the file name without path or extension
 */
Fl_String Project::basename() const {
  return fl_filename_setext(fl_filename_name(filename), "");
}

/**
 Generate the C++ source and header filenames and write those files.

 This function creates the source filename by setting the file
 extension to \c code_file_name and a header filename
 with the extension \c code_file_name which are both
 settable by the user.

 If the code filename has not been set yet, a "save file as" dialog will be
 presented to the user.

 In batch_mode, the function will either be silent, or, if opening or writing
 the files fails, write an error message to \c stderr and exit with exit code 1.

 In interactive mode, it will pop up an error message, or, if the user
 hasn't disabled that, pop up a confirmation message.

 \param[in] dont_show_completion_dialog don't show the completion dialog
 \return 1 if the operation failed, 0 if it succeeded
 */
int Project::write_code_files(bool dont_show_completion_dialog)
{
  // -- handle user interface issues
  flush_text_widgets();
  if (!filename) {
    fluid::Callbacks::save(0,0);
    if (!filename) return 1;
  }

  // -- generate the file names with absolute paths
  Fd_Code_Writer f { *this };
  Fl_String code_filename = codefile_path() + codefile_name();
  Fl_String header_filename = headerfile_path() + headerfile_name();

  // -- write the code and header files
  if (!Fluid.batch_mode) enter_project_dir();
  int x = f.write_code(code_filename.c_str(), header_filename.c_str());
  Fl_String code_filename_rel = fl_filename_relative(code_filename);
  Fl_String header_filename_rel = fl_filename_relative(header_filename);
  if (!Fluid.batch_mode) leave_project_dir();

  // -- print error message in batch mode or pop up an error or confirmation dialog box
  if (Fluid.batch_mode) {
    if (!x) {
      fprintf(stderr, "%s and %s: %s\n",
              code_filename_rel.c_str(),
              header_filename_rel.c_str(),
              strerror(errno));
      exit(1);
    }
  } else {
    if (!x) {
      fl_message("Can't write %s or %s: %s",
                 code_filename_rel.c_str(),
                 header_filename_rel.c_str(),
                 strerror(errno));
    } else {
      set_modflag(-1, 0);
      if (dont_show_completion_dialog==false && completion_button->value()) {
        fl_message("Wrote %s and %s",
                   code_filename_rel.c_str(),
                   header_filename_rel.c_str());
      }
    }
  }
  return 0;
}

/**
 Change the current working directory to the .fl project directory.

 Every call to enter_project_dir() must have a corresponding leave_project_dir()
 call. Enter and leave calls can be nested.

 The first call to enter_project_dir() remembers the original directory, usually
 the launch directory of the application. Nested calls will increment a nesting
 counter. When the nesting counter is back to 0, leave_project_dir() will return
 to the original directory.

 The global variable 'filename' must be set to the current project file with
 absolute or relative path information.

 \see leave_project_dir(), pwd, in_project_dir
 */
void Project::enter_project_dir() {
  if (in_project_dir<0) {
    fprintf(stderr, "** Fluid internal error: enter_project_dir() calls unmatched\n");
    return;
  }
  in_project_dir++;
  // check if we are already in the project dir and do nothing if so
  if (in_project_dir>1) return;
  // check if there is an active project, and do nothing if there is none
  if (!filename || !*filename) {
    fprintf(stderr, "** Fluid internal error: enter_project_dir() no filename set\n");
    return;
  }
  // set the current directory to the path of our .fl file
  Fl_String project_path = fl_filename_path(fl_filename_absolute(filename));
  if (fl_chdir(project_path.c_str()) == -1) {
    fprintf(stderr, "** Fluid internal error: enter_project_dir() can't chdir to %s: %s\n",
            project_path.c_str(), strerror(errno));
    return;
  }
}

/**
 Change the current working directory to the previous directory.
 \see enter_project_dir(), pwd, in_project_dir
 */
void Project::leave_project_dir() {
  if (in_project_dir == 0) {
    fprintf(stderr, "** Fluid internal error: leave_project_dir() calls unmatched\n");
    return;
  }
  in_project_dir--;
  // still nested, stay in the project directory
  if (in_project_dir > 0) return;
  // no longer nested, return to the original, usually the application working directory
  if (fl_chdir(Fluid.launch_path.c_str()) < 0) {
    fprintf(stderr, "** Fluid internal error: leave_project_dir() can't chdir back to %s : %s\n",
            Fluid.launch_path.c_str(), strerror(errno));
  }
}

/**
 Set the filename of the current .fl design.
 \param[in] c the new absolute filename and path
 */
void Project::set_filename(const char *c) {
  if (filename) ::free((void *)filename);
  filename = c ? fl_strdup(c) : nullptr;

  if (filename && !Fluid.batch_mode)
    Fluid.history.add(filename);

  set_modflag(modflag);
}

/**
 Save the current project to the file given by \c filename.

 This C++ function saves the current design to a file. Setting 
 \c ask_for_filename to true, or if no filename is set yet, it 
 will open a file chooser dialog. If the file already exists, it 
 prompts the user to confirm replacement. 
 
 After saving, it updates the filename and modification flags 
 if update_filename is true.

 "Save" sets ask_for_filename and update_filename to false.
 "Save As..." sets ask_for_filename and update_filename to true.
 "Save a Copy..." sets ask_for_filename to true and update_filename to false.

 \param[in] ask_for_filename if true, open a filechooser and warn if file exists.
 \param[in] update_filename if true, update filename and modification flags
 */
void Project::save(bool ask_for_filename, bool update_filename) {
  flush_text_widgets();
  Fl_Native_File_Chooser fnfc;
  const char *c = filename;
  if (ask_for_filename || !c || !*c) {
    fnfc.title("Save To:");
    fnfc.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
    fnfc.filter("FLUID Files\t*.f[ld]");
    if (fnfc.show() != 0) return;
    c = fnfc.filename();
    if (!fl_access(c, 0)) {
      Fl_String basename = fl_filename_name(Fl_String(c));
      if (fl_choice("The file \"%s\" already exists.\n"
                    "Do you want to replace it?", "Cancel",
                    "Replace", NULL, basename.c_str()) == 0) return;
    }

    if (update_filename) set_filename(c);
  }
  if (!write_file(c)) {
    fl_alert("Error writing %s: %s", c, strerror(errno));
    return;
  }

  if (update_filename) {
    set_modflag(0, 1);
    undo_save = undo_current;
  }
}

/**
 Load a project from the give file name and path.

 The project file is inserted at the currently selected type.

 If no filename is given, FLUID will open a file chooser dialog.

 \param[in] filename_arg path and name of the new project file
 \return false if the operation failed
 */
bool Project::merge_project_file(const Fl_String &filename_arg) {
  bool is_a_merge = (Fl_Type::first != NULL);
  Fl_String title = is_a_merge ? "Merge Project File" : "Open Project File";

  // ask for a filename if none was given
  Fl_String new_filename = filename_arg;
  if (new_filename.empty()) {
    new_filename = open_project_filechooser(title);
    if (new_filename.empty()) {
      return false;
    }
  }

  const char *c = new_filename.c_str();
  const char *oldfilename = filename;
  filename    = NULL;
  set_filename(c);
  if (is_a_merge) undo_checkpoint();
  undo_suspend();
  if (!read_file(c, is_a_merge)) {
    undo_resume();
    widget_browser->rebuild();
    g_project.update_settings_dialog();
    fl_message("Can't read %s: %s", c, strerror(errno));
    ::free((void *)filename);
    filename = oldfilename;
    if (main_window) set_modflag(modflag);
    return false;
  }
  undo_resume();
  widget_browser->rebuild();
  if (is_a_merge) {
    // Inserting a file; restore the original filename...
    set_filename(oldfilename);
    set_modflag(1);
  } else {
    // Loaded a file; free the old filename...
    set_modflag(0, 0);
    undo_clear();
  }
  if (oldfilename) free((void *)oldfilename);
  return true;
}
