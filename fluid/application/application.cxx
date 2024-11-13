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


#include "application/application.h"
#include "project/project.h"
#include "fluid.h"
#include "widget_browser.h"

#include <FL/fl_ask.H>
#include "../src/flstring.h"

#include <unistd.h>

using namespace fluid;

/**
 Clear the current project and create a new, empty one.

 If the current project was modified, FLUID will give the user the opportunity
 to save the old project first.

 \param[in] user_must_confirm if set, a confimation dialog is presented to the
    user before resetting the project. Default is `true`.
 \return false if the operation was canceled
 */
bool fluid::Application::new_project(bool user_must_confirm) {
  // verify user intention
  if ((user_must_confirm) &&  (project().confirm_clear() == false))
    return false;

  // clear the current project
  project().reset();
  set_filename(NULL);
  set_modflag(0, 0);
  widget_browser->rebuild();
  project().update_settings_dialog();

  // all is clear to continue
  return true;
}

/**
 * Return a reference to the current project.
 */
fluid::Project &fluid::Application::project() 
{ 
  return g_project; 
}


/**
 Generate a path to a directory for temporary data storage.
 The path is stored in g_tmpdir.
 */
void Application::create_tmpdir() {
  if (tmpdir_create_called)
    return;
  tmpdir_create_called = true;

  char buf[128];
#if _WIN32
  // The usual temp file locations on Windows are
  //    %system%\Windows\Temp
  //    %userprofiles%\AppData\Local
  // usually resolving into
  //    C:/Windows/Temp/
  //    C:\Users\<username>\AppData\Local\Temp
  fl_snprintf(buf, sizeof(buf)-1, "fluid-%d/", (long)GetCurrentProcessId());
  Fl_String name = buf;
  wchar_t tempdirW[FL_PATH_MAX+1];
  char tempdir[FL_PATH_MAX+1];
  unsigned len = GetTempPathW(FL_PATH_MAX, tempdirW);
  if (len == 0) {
    strcpy(tempdir, "c:/windows/temp/");
  } else {
    unsigned wn = fl_utf8fromwc(tempdir, FL_PATH_MAX, tempdirW, len);
    tempdir[wn] = 0;
  }
  Fl_String path = tempdir;
  end_with_slash(path);
  path += name;
  fl_make_path(path.c_str());
  if (fl_access(path.c_str(), 6) == 0) tmpdir_path = path;
#else
  fl_snprintf(buf, sizeof(buf)-1, "fluid-%d/", getpid());
  Fl_String name = buf;
  Fl_String path = fl_getenv("TMPDIR");
  if (!path.empty()) {
    end_with_slash(path);
    path += name;
    fl_make_path(path.c_str());
    if (fl_access(path.c_str(), 6) == 0) tmpdir_path = path;
  }
  if (tmpdir_path.empty()) {
    path = Fl_String("/tmp/") + name;
    fl_make_path(path.c_str());
    if (fl_access(path.c_str(), 6) == 0) tmpdir_path = path;
  }
#endif
  if (tmpdir_path.empty()) {
    char pbuf[FL_PATH_MAX+1];
    fluid_prefs.get_userdata_path(pbuf, FL_PATH_MAX);
    path = Fl_String(pbuf);
    end_with_slash(path);
    path += name;
    fl_make_path(path.c_str());
    if (fl_access(path.c_str(), 6) == 0) tmpdir_path = path;
  }
  if (tmpdir_path.empty()) {
    if (Fluid.batch_mode) {
      fprintf(stderr, "ERROR: Can't create directory for temporary data storage.\n");
    } else {
      fl_alert("Can't create directory for temporary data storage.");
    }
  }
}

/**
 Delete the temporary directory that was created in set_tmpdir.
 */
void Application::delete_tmpdir() {
  // was a temporary directory created
  if (!tmpdir_create_called)
    return;
  if (tmpdir_path.empty())
    return;

  // first delete all files that may still be left in the temp directory
  struct dirent **de;
  int n_de = fl_filename_list(tmpdir_path.c_str(), &de);
  if (n_de >= 0) {
    for (int i=0; i<n_de; i++) {
      Fl_String path = tmpdir_path + de[i]->d_name;
      fl_unlink(path.c_str());
    }
    fl_filename_free_list(&de, n_de);
  }

  // then delete the directory itself
  if (fl_rmdir(tmpdir_path.c_str()) < 0) {
    if (Fluid.batch_mode) {
      fprintf(stderr, "WARNING: Can't delete tmpdir '%s': %s", tmpdir_path.c_str(), strerror(errno));
    } else {
      fl_alert("WARNING: Can't delete tmpdir '%s': %s", tmpdir_path.c_str(), strerror(errno));
    }
  }
}

/**
 Return the path to a temporary directory for this instance of FLUID.
 Fluid will do its best to clear and delete this directory when exiting.
 \return the path to the temporary directory, ending in a '/', or and empty
      string if no directory could be created.
 */
const Fl_String &Application::get_tmpdir() {
  if (!tmpdir_create_called)
    create_tmpdir();
  return tmpdir_path;
}

