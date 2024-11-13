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

extern bool confirm_project_clear();

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
  if ((user_must_confirm) &&  (confirm_project_clear() == false))
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

