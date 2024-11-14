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

#include "fluid.h"

#include "application/application.h"
#include "project/project.h"
#include "Fl_Type.h"
#include "Fl_Function_Type.h"
#include "Fl_Group_Type.h"
#include "Fl_Window_Type.h"
#include "widget_browser.h"
#include "shell_command.h"
#include "factory.h"
#include "pixmaps.h"
#include "project/undo.h"
#include "file.h"
#include "code.h"
#include "mergeback.h"

#include "settings_panel.h"
#include "function_panel.h"
#include "codeview_panel.h"
#include "template_panel.h"
#include "about_panel.h"
#include "autodoc.h"

#include <FL/Fl.H>
#ifdef __APPLE__
#include <FL/platform.H> // for fl_open_callback
#endif
#include <FL/Fl_Help_Dialog.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Printer.H>
#include <FL/fl_string_functions.h>
#include <locale.h>     // setlocale()..
#include "../src/flstring.h"

extern "C"
{
#if defined(HAVE_LIBPNG) && defined(HAVE_LIBZ)
#  include <zlib.h>
#  ifdef HAVE_PNG_H
#    include <png.h>
#  else
#    include <libpng/png.h>
#  endif // HAVE_PNG_H
#endif // HAVE_LIBPNG && HAVE_LIBZ
}

/// \defgroup globals Fluid Global Variables, Functions and Callbacks
/// \{

//
// Globals..
//

//-> application::ui
/// FLUID-wide help dialog.
static Fl_Help_Dialog *help_dialog = NULL;

//-> application::ui
/// Main app window menu bar.
Fl_Menu_Bar *main_menubar = NULL;

//-> application::ui
/// Main app window.
Fl_Window *main_window;

//-> application
/// Fluid application preferences, always accessible, will be flushed when app closes.
Fl_Preferences fluid_prefs(Fl_Preferences::USER_L, "fltk.org", "fluid");

//-> eliminate! Use parameters.
/// This is set to create different labels when creating new widgets.
/// \todo Details unclear.
int reading_file = 0;

//-> application::ui
/// Menuitem to save a .fl design file, will be deactivated if the design is unchanged.
Fl_Menu_Item *save_item = NULL;

//-> application::ui
/// First Menuitem that shows the .fl design file history.
Fl_Menu_Item *history_item = NULL;

//-> application::ui
/// Menuitem to show or hide the widget bin, label will change if bin is visible.
Fl_Menu_Item *widgetbin_item = NULL;

//-> application::ui
/// Menuitem to show or hide the code view, label will change if view is visible.
Fl_Menu_Item *codeview_item = NULL;

//-> application::ui
/// Menuitem to show or hide the editing overlay, label will change if overlay visibility changes.
Fl_Menu_Item *overlay_item = NULL;

//-> application::ui
/// Menuitem to show or hide the editing guides, label will change if overlay visibility changes.
Fl_Menu_Item *guides_item = NULL;

//-> application::ui
/// Menuitem to show or hide the restricted area overlys, label will change if overlay visibility changes.
Fl_Menu_Item *restricted_item = NULL;

////////////////////////////////////////////////////////////////


/// Global reference to FLUID application
fluid::Application Fluid;

//-> tools/filenames
/**
 Make sure that a path name ends with a forward slash.
 \param[in] str directory or path name
 \return a new string, ending with a '/'
 */
Fl_String end_with_slash(const Fl_String &str) {
  char last = str[str.size()-1];
  if (last !='/' && last != '\\')
    return str + "/";
  else
    return str;
}

// ----

//-> application::ui
extern Fl_Window *the_panel;

//-> application::ui
/**
 Ensure that text widgets in the widget panel propagates apply current changes.
 By temporarily clearing the text focus, all text widgets with changed text
 will unfocus and call their respective callbacks, propagating those changes to
 their data set.
 */
void flush_text_widgets() {
  if (Fl::focus() && (Fl::focus()->top_window() == the_panel)) {
    Fl_Widget *old_focus = Fl::focus();
    Fl::focus(NULL); // trigger callback of the widget that is losing focus
    Fl::focus(old_focus);
  }
}

// ----

//-> tools::ui
/**
 Position the given window window based on entries in the app preferences.
 Customisable by user; feature can be switched off.
 The window is not shown or hidden by this function, but a value is returned
 to indicate the state to the caller.
 \param[in] w position this window
 \param[in] prefsName name of the preferences item that stores the window settings
 \param[in] Visible default value if window is hidden or shown
 \param[in] X, Y, W, H default size and position if nothing is specified in the preferences
 \return 1 if the caller should make the window visible, 0 if hidden.
 */
char position_window(Fl_Window *w, const char *prefsName, int Visible, int X, int Y, int W, int H) {
  Fl_Preferences pos(fluid_prefs, prefsName);
  if (prevpos_button->value()) {
    pos.get("x", X, X);
    pos.get("y", Y, Y);
    if ( W!=0 ) {
      pos.get("w", W, W);
      pos.get("h", H, H);
      w->resize( X, Y, W, H );
    }
    else
      w->position( X, Y );
  }
  pos.get("visible", Visible, Visible);
  return Visible;
}

//-> tools::ui
/**
 Save the position and visibility state of a window to the app preferences.
 \param[in] w save this window data
 \param[in] prefsName name of the preferences item that stores the window settings
 */
void save_position(Fl_Window *w, const char *prefsName) {
  Fl_Preferences pos(fluid_prefs, prefsName);
  pos.set("x", w->x());
  pos.set("y", w->y());
  pos.set("w", w->w());
  pos.set("h", w->h());
  pos.set("visible", (int)(w->shown() && w->visible()));
}

//-> external editor
/**
 Timer to watch for external editor modifications.

 If one or more external editors open, check if their files were modified.
 If so: reload to ram, update size/mtime records, and change fluid's
 'modified' state.
 */
static void external_editor_timer(void*) {
  int editors_open = ExternalCodeEditor::editors_open();
  if ( Fluid.args.debug ) printf("--- TIMER --- External editors open=%d\n", editors_open);
  if ( editors_open > 0 ) {
    // Walk tree looking for files modified by external editors.
    int modified = 0;
    for (Fl_Type *p = Fl_Type::first; p; p = p->next) {
      if ( p->is_a(ID_Code) ) {
        Fl_Code_Type *code = (Fl_Code_Type*)p;
        // Code changed by external editor?
        if ( code->handle_editor_changes() ) {  // updates ram, file size/mtime
          modified++;
        }
        if ( code->is_editing() ) {             // editor open?
          code->reap_editor();                  // Try to reap; maybe it recently closed
        }
      }
    }
    if ( modified ) Fluid.project().set_modflag(1);
  }
  // Repeat timeout if editors still open
  //    The ExternalCodeEditor class handles start/stopping timer, we just
  //    repeat_timeout() if it's already on. NOTE: above code may have reaped
  //    only open editor, which would disable further timeouts. So *recheck*
  //    if editors still open, to ensure we don't accidentally re-enable them.
  //
  if ( ExternalCodeEditor::editors_open() ) {
    Fl::repeat_timeout(2.0, external_editor_timer);
  }
}

//-> application::callbacks
/**
 Save the current design to the file given by \c filename.

 \param[in] v v is (void *)1 for "Save As...", and (void *)2 for "Save a Copy..."
 */
void fluid::Callbacks::save(Fl_Widget *, void *v) {
  Fluid.project().save(v != NULL, v != (void *)2);
}

//-> application::callbacks
/**
 Save a design template.
 \todo We should document the concept of templates.
 */
void save_template_cb(Fl_Widget *, void *) {
  // Setup the template panel...
  if (!template_panel) make_template_panel();

  template_clear();
  template_browser->add("New Template");
  template_load();

  template_name->show();
  template_name->value("");

  template_instance->hide();

  template_delete->show();
  template_delete->deactivate();

  template_submit->label("Save");
  template_submit->deactivate();

  template_panel->label("Save Template");

  // Show the panel and wait for the user to do something...
  template_panel->show();
  while (template_panel->shown()) Fl::wait();

  // Get the template name, return if it is empty...
  const char *c = template_name->value();
  if (!c || !*c) return;

  // Convert template name to filename_with_underscores
  char savename[FL_PATH_MAX], *saveptr;
  strlcpy(savename, c, sizeof(savename));
  for (saveptr = savename; *saveptr; saveptr ++) {
    if (isspace(*saveptr)) *saveptr = '_';
  }

  // Find the templates directory...
  char filename[FL_PATH_MAX];
  fluid_prefs.getUserdataPath(filename, sizeof(filename));

  strlcat(filename, "templates", sizeof(filename));
  if (fl_access(filename, 0)) fl_make_path(filename);

  strlcat(filename, "/", sizeof(filename));
  strlcat(filename, savename, sizeof(filename));

  char *ext = filename + strlen(filename);
  if (ext >= (filename + sizeof(filename) - 5)) {
    fl_alert("The template name \"%s\" is too long!", c);
    return;
  }

  // Save the .fl file...
  strcpy(ext, ".fl");

  if (!fl_access(filename, 0)) {
    if (fl_choice("The template \"%s\" already exists.\n"
                  "Do you want to replace it?", "Cancel",
                  "Replace", NULL, c) == 0) return;
  }

  if (!write_file(filename)) {
    fl_alert("Error writing %s: %s", filename, strerror(errno));
    return;
  }

#if defined(HAVE_LIBPNG) && defined(HAVE_LIBZ)
  // Get the screenshot, if any...
  Fl_Type *t;

  for (t = Fl_Type::first; t; t = t->next) {
    // Find the first window...
    if (t->is_a(ID_Window)) break;
  }

  if (!t) return;

  // Grab a screenshot...
  Fl_Window_Type *wt = (Fl_Window_Type *)t;
  uchar *pixels;
  int w, h;

  if ((pixels = wt->read_image(w, h)) == NULL) return;

  // Save to a PNG file...
  strcpy(ext, ".png");

  errno = 0;
  if (fl_write_png(filename, pixels, w, h, 3) != 0) {
    delete[] pixels;
    fl_alert("Error writing %s: %s", filename, strerror(errno));
    return;
  }

#  if 0 // The original PPM output code...
  strcpy(ext, ".ppm");
  fp = fl_fopen(filename, "wb");
  fprintf(fp, "P6\n%d %d 255\n", w, h);
  fwrite(pixels, w * h, 3, fp);
  fclose(fp);
#  endif // 0

  delete[] pixels;
#endif // HAVE_LIBPNG && HAVE_LIBZ
}

void revert_cb(Fl_Widget *,void *) {
  Fluid.project().revert(); 
}

//-> application::callbacks
/**
 Exit Fluid; we hope you had a nice experience.
 If the design was modified, a dialog will ask for confirmation.
 */
void exit_cb(Fl_Widget *,void *) {
  if (shell_command_running()) {
    int choice = fl_choice("Previous shell command still running!",
                           "Cancel",
                           "Exit",
                           NULL);
    if (choice == 0) { // user chose to cancel the exit operation
      return;
    }
  }

  flush_text_widgets();

  // verify user intention
  if (Fluid.project().confirm_clear() == false)
    return;

  // Stop any external editor update timers
  ExternalCodeEditor::stop_update_timer();

  save_position(main_window,"main_window_pos");

  if (widgetbin_panel) {
    save_position(widgetbin_panel,"widgetbin_pos");
    delete widgetbin_panel;
  }
  if (codeview_panel) {
    Fl_Preferences svp(fluid_prefs, "codeview");
    svp.set("autorefresh", cv_autorefresh->value());
    svp.set("autoposition", cv_autoposition->value());
    svp.set("tab", cv_tab->find(cv_tab->value()));
    svp.set("code_choice", cv_code_choice);
    save_position(codeview_panel,"codeview_pos");
    delete codeview_panel;
    codeview_panel = 0;
  }
  if (shell_run_window) {
    save_position(shell_run_window,"shell_run_Window_pos");
  }

  if (about_panel)
    delete about_panel;
  if (help_dialog)
    delete help_dialog;

  if (g_shell_config)
    g_shell_config->write(fluid_prefs, Fd_Tool_Store::USER);
  g_layout_list.write(fluid_prefs, Fd_Tool_Store::USER);

  undo_clear();

  // Destroy tree
  //    Doing so causes dtors to automatically close all external editors
  //    and cleans up editor tmp files. Then remove fluid tmpdir /last/.
  g_project.reset();
  ExternalCodeEditor::tmpdir_clear();
  Fluid.delete_tmpdir();

  exit(0);
}

//-> application::callbacks
/**
 Open the template browser and load a new file from templates.

 If the current project was modified, FLUID will give the user the opportunity
 to save the old project first.

 \return false if the operation was canceled or failed otherwise
 */
bool new_project_from_template() {
  // clear the current project first
  if (Fluid.new_project() == false)
    return false;

  // Setup the template panel...
  if (!template_panel) make_template_panel();

  template_clear();
  template_browser->add("Blank");
  template_load();

  template_name->hide();
  template_name->value("");

  template_instance->show();
  template_instance->deactivate();
  template_instance->value("");

  template_delete->show();

  template_submit->label("New");
  template_submit->deactivate();

  template_panel->label("New");

  //if ( template_browser->size() == 1 ) { // only one item?
  template_browser->value(1);          // select it
  template_browser->do_callback();
  //}

  // Show the panel and wait for the user to do something...
  template_panel->show();
  while (template_panel->shown()) Fl::wait();

  // See if the user chose anything...
  int item = template_browser->value();
  if (item < 1) return false;

  // Load the template, if any...
  const char *tname = (const char *)template_browser->data(item);

  if (tname) {
    // Grab the instance name...
    const char *iname = template_instance->value();

    if (iname && *iname) {
      // Copy the template to a temp file, then read it in...
      char line[1024], *ptr, *next;
      FILE *infile, *outfile;

      if ((infile = fl_fopen(tname, "rb")) == NULL) {
        fl_alert("Error reading template file \"%s\":\n%s", tname,
                 strerror(errno));
        Fluid.project().set_modflag(0);
        undo_clear();
        return false;
      }

      if ((outfile = fl_fopen(Fluid.cutfname(1), "wb")) == NULL) {
        fl_alert("Error writing buffer file \"%s\":\n%s", Fluid.cutfname(1),
                 strerror(errno));
        fclose(infile);
        Fluid.project().set_modflag(0);
        undo_clear();
        return false;
      }

      while (fgets(line, sizeof(line), infile)) {
        // Replace @INSTANCE@ with the instance name...
        for (ptr = line; (next = strstr(ptr, "@INSTANCE@")) != NULL; ptr = next + 10) {
          fwrite(ptr, next - ptr, 1, outfile);
          fputs(iname, outfile);
        }

        fputs(ptr, outfile);
      }

      fclose(infile);
      fclose(outfile);

      undo_suspend();
      read_file(Fluid.cutfname(1), 0);
      fl_unlink(Fluid.cutfname(1));
      undo_resume();
    } else {
      // No instance name, so read the template without replacements...
      undo_suspend();
      read_file(tname, 0);
      undo_resume();
    }
  }

  widget_browser->rebuild();
  g_project.update_settings_dialog();
  Fluid.project().set_modflag(0);
  undo_clear();

  return true;
}

#ifdef __APPLE__
/**
 Handle app launch with an associated filename (macOS only).
 Should there be a modified design already, Fluid asks for user confirmation.
 \param[in] c the filename of the new design
 */
void apple_open_cb(const char *c) {
  Fluid.open_project_file(Fl_String(c));
}
#endif // __APPLE__


//-> application::callbacks
/**
 Callback to write C++ code and header files.
 */
void write_cb(Fl_Widget *, void *) {
    g_project.write_code_files();
}

#if 0
// Matt: disabled
/**
 Merge the possibly modified content of code files back into the project.
 */
int mergeback_code_files()
{
  flush_text_widgets();
  if (!filename) return 1;
  if (!g_project.write_mergeback_data) {
    fl_message("MergeBack is not enabled for this project.\n"
               "Please enable MergeBack in the project settings\n"
               "dialog and re-save the project file and the code.");
    return 0;
  }

  Fl_String proj_filename = g_project.projectfile_path() + g_project.projectfile_name();
  Fl_String code_filename;
#if 1
  if (!Fluid.batch_mode) {
    Fl_Preferences build_records(Fl_Preferences::USER_L, "fltk.org", "fluid-build");
    Fl_Preferences path(build_records, proj_filename.c_str());
    int i, n = proj_filename.size();
    for (i=0; i<n; i++) if (proj_filename[i]=='\\') proj_filename[i] = '/';
    preferences_get(path, "code", code_filename, "");
  }
#endif
  if (code_filename.empty())
    code_filename = g_project.codefile_path() + g_project.codefile_name();
  if (!Fluid.batch_mode) Fluid.project().enter_project_dir();
  int c = merge_back(code_filename, proj_filename, FD_MERGEBACK_INTERACTIVE);
  if (!Fluid.batch_mode) leave_project_dir();

  if (c==0) fl_message("Comparing\n  \"%s\"\nto\n  \"%s\"\n\n"
                       "MergeBack found no external modifications\n"
                       "in the source code.",
                       code_filename.c_str(), proj_filename.c_str());
  if (c==-2) fl_message("No corresponding source code file found.");
  return c;
}

void mergeback_cb(Fl_Widget *, void *) {
  mergeback_code_files();
}
#endif

void write_strings_cb(Fl_Widget *, void *) {
  Fluid.project().write_strings();
}

//-> application::callbacks
/**
 Show the editor for the \c current Fl_Type.
 */
void openwidget_cb(Fl_Widget *, void *) {
  if (!Fl_Type::current) {
    fl_message("Please select a widget");
    return;
  }
  Fl_Type::current->open();
}

void copy_cb(Fl_Widget*, void*) {
  Fluid.project().copy();
}
 
/**
 User chose to cut the currently selected widgets.
 */
void cut_cb(Fl_Widget *, void *) {
  Fluid.project().cut();
}

void delete_cb(Fl_Widget *, void *) {
  Fluid.project().user_delete();
}

void paste_cb(Fl_Widget*, void*) {
  Fluid.project().paste();
}

/**
 Duplicate the selected widgets.

 This code is a bit complex because it needs to find the last selected
 widget with the lowest level, so that the new widgets are inserted after
 this one.
 */
void duplicate_cb(Fl_Widget*, void*) {
  Fluid.project().duplicate();
}

//-> application::callbacks
/**
 User wants to sort selected widgets by y coordinate.
 */
static void sort_cb(Fl_Widget *,void *) {
  undo_checkpoint();
  sort((Fl_Type*)NULL);
  widget_browser->rebuild();
  Fluid.project().set_modflag(1);
}

//-> application::callbacks
/**
 Open the "About" dialog.
 */
void about_cb(Fl_Widget *, void *) {
  if (!about_panel) make_about_panel();
  about_panel->show();
}

//-> application::ui
/**
 Open a dialog to show the HTML help page form the FLTK documentation folder.
 \param[in] name name of the HTML help file.
 */
void show_help(const char *name) {
  const char    *docdir;
  char          helpname[FL_PATH_MAX];

  if (!help_dialog) help_dialog = new Fl_Help_Dialog();

  if ((docdir = fl_getenv("FLTK_DOCDIR")) == NULL) {
    docdir = FLTK_DOCDIR;
  }
  snprintf(helpname, sizeof(helpname), "%s/%s", docdir, name);

  // make sure that we can read the file
  FILE *f = fopen(helpname, "rb");
  if (f) {
    fclose(f);
    help_dialog->load(helpname);
  } else {
    // if we can not read the file, we display the canned version instead
    // or ask the native browser to open the page on www.fltk.org
    if (strcmp(name, "fluid.html")==0) {
      if (!Fl_Shared_Image::find("embedded:/fluid_flow_chart_800.png"))
        new Fl_PNG_Image("embedded:/fluid_flow_chart_800.png", fluid_flow_chart_800_png, sizeof(fluid_flow_chart_800_png));
      help_dialog->value
      (
       "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n"
       "<html><head><title>FLTK: Programming with FLUID</title></head><body>\n"
       "<h2>What is FLUID?</h2>\n"
       "The Fast Light User Interface Designer, or FLUID, is a graphical editor "
       "that is used to produce FLTK source code. FLUID edits and saves its state "
       "in <code>.fl</code> files. These files are text, and you can (with care) "
       "edit them in a text editor, perhaps to get some special effects.<p>\n"
       "FLUID can \"compile\" the <code>.fl</code> file into a <code>.cxx</code> "
       "and a <code>.h</code> file. The <code>.cxx</code> file defines all the "
       "objects from the <code>.fl</code> file and the <code>.h</code> file "
       "declares all the global ones. FLUID also supports localization "
       "(Internationalization) of label strings using message files and the GNU "
       "gettext or POSIX catgets interfaces.<p>\n"
       "A simple program can be made by putting all your code (including a <code>"
       "main()</code> function) into the <code>.fl</code> file and thus making the "
       "<code>.cxx</code> file a single source file to compile. Most programs are "
       "more complex than this, so you write other <code>.cxx</code> files that "
       "call the FLUID functions. These <code>.cxx</code> files must <code>"
       "#include</code> the <code>.h</code> file or they can <code>#include</code> "
       "the <code>.cxx</code> file so it still appears to be a single source file.<p>"
       "<img src=\"embedded:/fluid_flow_chart_800.png\"></p>"
       "<p>More information is available online at <a href="
       "\"https://www.fltk.org/doc-1.4/fluid.html\">https://www.fltk.org/</a>"
       "</body></html>"
       );
    } else if (strcmp(name, "license.html")==0) {
      fl_open_uri("https://www.fltk.org/doc-1.4/license.html");
      return;
    } else if (strcmp(name, "index.html")==0) {
      fl_open_uri("https://www.fltk.org/doc-1.4/index.html");
      return;
    } else {
      snprintf(helpname, sizeof(helpname), "https://www.fltk.org/%s", name);
      fl_open_uri(helpname);
      return;
    }
  }
  help_dialog->show();
}

//-> application::callbacks
/**
 User wants help on Fluid.
 */
void help_cb(Fl_Widget *, void *) {
  show_help("fluid.html");
}

//-> application::callbacks
/**
 User wants to see the Fluid manual.
 */
void manual_cb(Fl_Widget *, void *) {
  show_help("index.html");
}

// ---- Printing

//-> application::callbacks
/**
 Open the dialog to allow the user to print the current window.
 */
void print_menu_cb(Fl_Widget *, void *) {
  int w, h, ww, hh;
  int frompage, topage;
  Fl_Type       *t;                     // Current widget
  int           num_windows;            // Number of windows
  Fl_Window_Type *windows[1000];        // Windows to print
  int           winpage;                // Current window page
  Fl_Window *win;

  for (t = Fl_Type::first, num_windows = 0; t; t = t->next) {
    if (t->is_a(ID_Window)) {
      windows[num_windows] = (Fl_Window_Type *)t;
      if (!((Fl_Window*)(windows[num_windows]->o))->shown()) continue;
      num_windows ++;
    }
  }

  Fl_Printer printjob;
  if ( printjob.start_job(num_windows, &frompage, &topage) ) return;
  int pagecount = 0;
  for (winpage = 0; winpage < num_windows; winpage++) {
    float scale = 1, scale_x = 1, scale_y = 1;
    if (winpage+1 < frompage || winpage+1 > topage) continue;
    printjob.start_page();
    printjob.printable_rect(&w, &h);

    // Get the time and date...
    time_t curtime = time(NULL);
    struct tm *curdate = localtime(&curtime);
    char date[1024];
    strftime(date, sizeof(date), "%c", curdate);
    fl_font(FL_HELVETICA, 12);
    fl_color(FL_BLACK);
    fl_draw(date, (w - (int)fl_width(date))/2, fl_height());
    sprintf(date, "%d/%d", ++pagecount, topage-frompage+1);
    fl_draw(date, w - (int)fl_width(date), fl_height());

    // Get the base filename...
    Fl_String basename = fl_filename_name(Fl_String(Fluid.project().filename));
    fl_draw(basename.c_str(), 0, fl_height());

    // print centered and scaled to fit in the page
    win = (Fl_Window*)windows[winpage]->o;
    ww = win->decorated_w();
    if(ww > w) scale_x = float(w)/ww;
    hh = win->decorated_h();
    if(hh > h) scale_y = float(h)/hh;
    if (scale_x < scale) scale = scale_x;
    if (scale_y < scale) scale = scale_y;
    if (scale < 1) {
      printjob.scale(scale);
      printjob.printable_rect(&w, &h);
    }
    printjob.origin(w/2, h/2);
    printjob.print_window(win, -ww/2, -hh/2);
    printjob.end_page();
  }
  printjob.end_job();
}

// ---- Main menu bar

extern void select_layout_preset_cb(Fl_Widget *, void *user_data);
extern void layout_suite_marker(Fl_Widget *, void *user_data);

static void menu_file_new_cb(Fl_Widget *, void *) { Fluid.new_project(); }
static void menu_file_new_from_template_cb(Fl_Widget *, void *) { new_project_from_template(); }
static void menu_file_open_cb(Fl_Widget *, void *) { Fluid.open_project_file(""); }
static void menu_file_insert_cb(Fl_Widget *, void *) { Fluid.project().merge_project_file(""); }
static void menu_file_open_history_cb(Fl_Widget *, void *v) { Fluid.open_project_file(Fl_String((const char*)v)); }
static void menu_layout_sync_resize_cb(Fl_Menu_ *m, void*) {
  if (m->mvalue()->value()) Fl_Type::allow_layout = 1; else Fl_Type::allow_layout = 0; }

//-> application::ui
/**
 This is the main Fluid menu.

 Design history is manipulated right inside this menu structure.
 Some menu items change or deactivate correctly, but most items just trigger
 various callbacks.

 \c New_Menu creates new widgets and is explained in detail in another location.

 \see New_Menu
 \todo This menu needs some major modernization. Menus are too long and their
    sorting is not always obvious.
 \todo Shortcuts are all over the place (Alt, Ctrl, Command, Shift-Ctrl,
    function keys), and there should be a help page listing all shortcuts.
 */
Fl_Menu_Item Main_Menu[] = {
{"&File",0,0,0,FL_SUBMENU},
  {"&New", FL_COMMAND+'n', menu_file_new_cb},
  {"&Open...", FL_COMMAND+'o', menu_file_open_cb},
  {"&Insert...", FL_COMMAND+'i', menu_file_insert_cb, 0, FL_MENU_DIVIDER},
  {"&Save", FL_COMMAND+'s', fluid::Callbacks::save, 0},
  {"Save &As...", FL_COMMAND+FL_SHIFT+'s', fluid::Callbacks::save, (void*)1},
  {"Sa&ve A Copy...", 0, fluid::Callbacks::save, (void*)2},
  {"&Revert...", 0, revert_cb, 0, FL_MENU_DIVIDER},
  {"New &From Template...", FL_COMMAND+'N', menu_file_new_from_template_cb, 0},
  {"Save As &Template...", 0, save_template_cb, 0, FL_MENU_DIVIDER},
  {"&Print...", FL_COMMAND+'p', print_menu_cb},
  {"Write &Code", FL_COMMAND+FL_SHIFT+'c', write_cb, 0},
// Matt: disabled {"MergeBack Code", FL_COMMAND+FL_SHIFT+'m', mergeback_cb, 0},
  {"&Write Strings", FL_COMMAND+FL_SHIFT+'w', write_strings_cb, 0, FL_MENU_DIVIDER},
  {Fluid.history.short_path[0], FL_COMMAND+'1', menu_file_open_history_cb, Fluid.history.full_path[0]},
  {Fluid.history.short_path[1], FL_COMMAND+'2', menu_file_open_history_cb, Fluid.history.full_path[1]},
  {Fluid.history.short_path[2], FL_COMMAND+'3', menu_file_open_history_cb, Fluid.history.full_path[2]},
  {Fluid.history.short_path[3], FL_COMMAND+'4', menu_file_open_history_cb, Fluid.history.full_path[3]},
  {Fluid.history.short_path[4], FL_COMMAND+'5', menu_file_open_history_cb, Fluid.history.full_path[4]},
  {Fluid.history.short_path[5], FL_COMMAND+'6', menu_file_open_history_cb, Fluid.history.full_path[5]},
  {Fluid.history.short_path[6], FL_COMMAND+'7', menu_file_open_history_cb, Fluid.history.full_path[6]},
  {Fluid.history.short_path[7], FL_COMMAND+'8', menu_file_open_history_cb, Fluid.history.full_path[7]},
  {Fluid.history.short_path[8], FL_COMMAND+'9', menu_file_open_history_cb, Fluid.history.full_path[8]},
  {Fluid.history.short_path[9], 0, menu_file_open_history_cb, Fluid.history.full_path[9], FL_MENU_DIVIDER},
  {"&Quit", FL_COMMAND+'q', exit_cb},
  {0},
{"&Edit",0,0,0,FL_SUBMENU},
  {"&Undo", FL_COMMAND+'z', undo_cb},
  {"&Redo", FL_COMMAND+FL_SHIFT+'z', redo_cb, 0, FL_MENU_DIVIDER},
  {"C&ut", FL_COMMAND+'x', cut_cb},
  {"&Copy", FL_COMMAND+'c', copy_cb},
  {"&Paste", FL_COMMAND+'v', paste_cb},
  {"Dup&licate", FL_COMMAND+'u', duplicate_cb},
  {"&Delete", FL_Delete, delete_cb, 0, FL_MENU_DIVIDER},
  {"Select &All", FL_COMMAND+'a', select_all_cb},
  {"Select &None", FL_COMMAND+FL_SHIFT+'a', select_none_cb, 0, FL_MENU_DIVIDER},
  {"Pr&operties...", FL_F+1, openwidget_cb},
  {"&Sort",0,sort_cb},
  {"&Earlier", FL_F+2, earlier_cb},
  {"&Later", FL_F+3, later_cb},
  {"&Group", FL_F+7, group_cb},
  {"Ung&roup", FL_F+8, ungroup_cb,0, FL_MENU_DIVIDER},
  {"Hide O&verlays",FL_COMMAND+FL_SHIFT+'o',toggle_overlays},
  {"Hide Guides",FL_COMMAND+FL_SHIFT+'g',toggle_guides},
  {"Hide Restricted",FL_COMMAND+FL_SHIFT+'r',toggle_restricted},
  {"Show Widget &Bin...",FL_ALT+'b',toggle_widgetbin_cb},
  {"Show Code View",FL_ALT+'c', (Fl_Callback*)toggle_codeview_cb, 0, FL_MENU_DIVIDER},
  {"Settings...",FL_ALT+'p',show_settings_cb},
  {0},
{"&New", 0, 0, (void *)New_Menu, FL_SUBMENU_POINTER},
{"&Layout",0,0,0,FL_SUBMENU},
  {"&Align",0,0,0,FL_SUBMENU},
    {"&Left",0,(Fl_Callback *)align_widget_cb,(void*)10},
    {"&Center",0,(Fl_Callback *)align_widget_cb,(void*)11},
    {"&Right",0,(Fl_Callback *)align_widget_cb,(void*)12},
    {"&Top",0,(Fl_Callback *)align_widget_cb,(void*)13},
    {"&Middle",0,(Fl_Callback *)align_widget_cb,(void*)14},
    {"&Bottom",0,(Fl_Callback *)align_widget_cb,(void*)15},
    {0},
  {"&Space Evenly",0,0,0,FL_SUBMENU},
    {"&Across",0,(Fl_Callback *)align_widget_cb,(void*)20},
    {"&Down",0,(Fl_Callback *)align_widget_cb,(void*)21},
    {0},
  {"&Make Same Size",0,0,0,FL_SUBMENU},
    {"&Width",0,(Fl_Callback *)align_widget_cb,(void*)30},
    {"&Height",0,(Fl_Callback *)align_widget_cb,(void*)31},
    {"&Both",0,(Fl_Callback *)align_widget_cb,(void*)32},
    {0},
  {"&Center In Group",0,0,0,FL_SUBMENU},
    {"&Horizontal",0,(Fl_Callback *)align_widget_cb,(void*)40},
    {"&Vertical",0,(Fl_Callback *)align_widget_cb,(void*)41},
    {0},
  {"Synchronized Resize", 0, (Fl_Callback*)menu_layout_sync_resize_cb, NULL, FL_MENU_TOGGLE|FL_MENU_DIVIDER },
  {"&Grid and Size Settings...",FL_COMMAND+'g',show_grid_cb, NULL, FL_MENU_DIVIDER},
  {"Presets", 0, layout_suite_marker, (void*)main_layout_submenu_, FL_SUBMENU_POINTER },
  {"Application", 0, select_layout_preset_cb, (void*)0, FL_MENU_RADIO|FL_MENU_VALUE },
  {"Dialog",      0, select_layout_preset_cb, (void*)1, FL_MENU_RADIO },
  {"Toolbox",     0, select_layout_preset_cb, (void*)2, FL_MENU_RADIO },
  {0},
{"&Shell", 0, Fd_Shell_Command_List::menu_marker, (void*)Fd_Shell_Command_List::default_menu, FL_SUBMENU_POINTER},
{"&Help",0,0,0,FL_SUBMENU},
  {"&Rapid development with FLUID...",0,help_cb},
  {"&FLTK Programmers Manual...",0,manual_cb, 0, FL_MENU_DIVIDER},
  {"&About FLUID...",0,about_cb},
  {0},
{0}};

//-> application::callbacks
/**
 Change the app's and hence preview the design's scheme.

 The scheme setting is stored in the app preferences
 - in key \p 'scheme_name' since 1.4.0
 - in key \p 'scheme' (index: 0 - 4) in 1.3.x

 This callback is triggered by changing the scheme in the
 Fl_Scheme_Choice widget (\p Edit/GUI Settings).

 \param[in] choice the calling widget

 \see init_scheme() for choice values and backwards compatibility
 */
void scheme_cb(Fl_Scheme_Choice *choice, void *) {
  if (Fluid.batch_mode)
    return;

  // set the new scheme only if the scheme was changed
  const char *new_scheme = choice->text(choice->value());

  if (Fl::is_scheme(new_scheme))
    return;

  Fl::scheme(new_scheme);
  fluid_prefs.set("scheme_name", new_scheme);

  // Backwards compatibility: store 1.3 scheme index (1-4).
  // We assume that index 0-3 (base, plastic, gtk+, gleam) are in the
  // same order as in 1.3.x (index 1-4), higher values are ignored

  int scheme_index = scheme_choice->value();
  if (scheme_index <= 3)                          // max. index for 1.3.x (Gleam)
    fluid_prefs.set("scheme", scheme_index + 1);  // compensate for different indexing
}

/**
  Read Fluid's scheme preferences and set the app's scheme.

  Since FLTK 1.4.0 the scheme \b name is stored as a character string
  with key "scheme_name" in the preference database.

  In FLTK 1.3.x the scheme preference was stored as an integer index
  with key "scheme" in the database. The known schemes were hardcoded in
  Fluid's sources (here for reference):

    | Index | 1.3 Scheme Name | Choice | 1.4 Scheme Name |
    |-------|-----------------|-------|-----------------|
    | 0 | Default (same as None) | n/a | n/a |
    | 1 | None (same as Default) | 0 | base |
    | 2 | Plastic | 1 | plastic |
    | 3 | GTK+ | 2 | gtk+ |
    | 4 | Gleam | 3 | gleam |
    | n/a | n/a | 4 | oxy |

  The new Fluid tries to keep backwards compatibility and reads both
  keys (\p scheme and \p scheme_name). If the latter is defined, it is used.
  If not the old \p scheme (index) is used - but we need to subtract one to
  get the new Fl_Scheme_Choice index (column "Choice" above).
*/
void init_scheme() {
  int scheme_index = 0;                     // scheme index for backwards compatibility (1.3.x)
  char *scheme_name = 0;                    // scheme name since 1.4.0
  fluid_prefs.get("scheme_name", scheme_name, "XXX"); // XXX means: not set => fallback 1.3.x
  if (!strcmp(scheme_name, "XXX")) {
    fluid_prefs.get("scheme", scheme_index, 0);
    if (scheme_index > 0) {
      scheme_index--;
      scheme_choice->value(scheme_index);   // set the choice value
    }
    if (scheme_index < 0)
      scheme_index = 0;
    else if (scheme_index > scheme_choice->size() - 1)
      scheme_index = 0;
    scheme_name = const_cast<char *>(scheme_choice->text(scheme_index));
    fluid_prefs.set("scheme_name", scheme_name);
  }
  // Set the new scheme only if it was not overridden by the -scheme
  // command line option
  if (Fl::scheme() == NULL) {
    Fl::scheme(scheme_name);
  }
  free(scheme_name);
}

//-> panels::widgetbin::callbacks
/**
 Show or hide the widget bin.
 The state is stored in the app preferences.
 */
void toggle_widgetbin_cb(Fl_Widget *, void *) {
  if (!widgetbin_panel) {
    make_widgetbin();
    if (!position_window(widgetbin_panel,"widgetbin_pos", 1, 320, 30)) return;
  }

  if (widgetbin_panel->visible()) {
    widgetbin_panel->hide();
    widgetbin_item->label("Show Widget &Bin...");
  } else {
    widgetbin_panel->show();
    widgetbin_item->label("Hide Widget &Bin");
  }
}

//-> application::callbacks
/**
 Show or hide the code preview window.
 */
void toggle_codeview_cb(Fl_Double_Window *, void *) {
  codeview_toggle_visibility();
}

//-> application::callbacks
/**
 Show or hide the code preview window, button callback.
 */
void toggle_codeview_b_cb(Fl_Button*, void *) {
  codeview_toggle_visibility();
}

//-> application::ui
/**
 Build the main app window and create a few other dialogs.
 */
void make_main_window() {
  if (!Fluid.batch_mode) {
    fluid_prefs.get("show_guides", Fluid.settings.show_guides, 1);
    fluid_prefs.get("show_restricted", Fluid.settings.show_restricted, 1);
    fluid_prefs.get("show_ghosted_outline", Fluid.settings.show_ghosted_outline, 0);
    fluid_prefs.get("show_comments", Fluid.settings.show_comments, 1);
    make_shell_window();
  }

  if (!main_window) {
    Fl_Widget *o;
    loadPixmaps();
    main_window = new Fl_Double_Window(WINWIDTH,WINHEIGHT,"fluid");
    main_window->box(FL_NO_BOX);
    o = make_widget_browser(0,MENUHEIGHT,BROWSERWIDTH,BROWSERHEIGHT);
    o->box(FL_FLAT_BOX);
    o->tooltip("Double-click to view or change an item.");
    main_window->resizable(o);
    main_menubar = new Fl_Menu_Bar(0,0,BROWSERWIDTH,MENUHEIGHT);
    main_menubar->menu(Main_Menu);
    // quick access to all dynamic menu items
    save_item = (Fl_Menu_Item*)main_menubar->find_item(fluid::Callbacks::save);
    history_item = (Fl_Menu_Item*)main_menubar->find_item(menu_file_open_history_cb);
    widgetbin_item = (Fl_Menu_Item*)main_menubar->find_item(toggle_widgetbin_cb);
    codeview_item = (Fl_Menu_Item*)main_menubar->find_item((Fl_Callback*)toggle_codeview_cb);
    overlay_item = (Fl_Menu_Item*)main_menubar->find_item((Fl_Callback*)toggle_overlays);
    guides_item = (Fl_Menu_Item*)main_menubar->find_item((Fl_Callback*)toggle_guides);
    restricted_item = (Fl_Menu_Item*)main_menubar->find_item((Fl_Callback*)toggle_restricted);
    main_menubar->global();
    fill_in_New_Menu();
    main_window->end();
  }

  if (!Fluid.batch_mode) {
    Fluid.history.load();
    g_shell_config = new Fd_Shell_Command_List;
    widget_browser->load_prefs();
    make_settings_window();
  }
}

// ---- Main program entry point

#if ! (defined(_WIN32) && !defined (__CYGWIN__))

int quit_flag = 0;
#include <signal.h>
#ifdef _sigargs
#define SIGARG _sigargs
#else
#ifdef __sigargs
#define SIGARG __sigargs
#else
#define SIGARG int // you may need to fix this for older systems
#endif
#endif

extern "C" {
static void sigint(SIGARG) {
  signal(SIGINT,sigint);
  quit_flag = 1;
}
}

#endif

/**
 Start Fluid.

 Fluid can run in interactive mode with a full user interface to design new
 user interfaces and write the C++ files to manage them,

 Fluid can run form the command line in batch mode to convert .fl design files
 into C++ source and header files. In batch mode, no display is needed,
 particularly no X11 connection will be attempted on Linux/Unix.

 \param[in] argc number of arguments in the list
 \param[in] argv pointer to an array of arguments
 \return in batch mode, an error code will be returned via \c exit() . This
    function return 1, if there was an error in the parameters list.
 \todo On MSWindows, Fluid can under certain conditions open a dialog box, even
    in batch mode. Is that intentional? Does it circumvent issues with Windows'
 stderr and stdout?
 */
int main(int argc,char **argv) {

  setlocale(LC_ALL, "");      // enable multi-language errors in file chooser
  setlocale(LC_NUMERIC, "C"); // make sure numeric values are written correctly
  Fluid.launch_path = end_with_slash(fl_getcwd()); // store the current path at launch

  int project_index = Fluid.args.read(argc, argv);
  if (project_index == -1) {
    return 1;
  }

  const char *c = NULL;
  if (Fluid.args.autodoc_path.empty())
    c = argv[project_index];

  fl_register_images();

  make_main_window();

  if (c) Fluid.project().set_filename(c);
  if (!Fluid.batch_mode) {
#ifdef __APPLE__
    fl_open_callback(apple_open_cb);
#endif // __APPLE__
    Fl::visual((Fl_Mode)(FL_DOUBLE|FL_INDEX));
    Fl_File_Icon::load_system_icons();
    main_window->callback(exit_cb);
    position_window(main_window,"main_window_pos", 1, 10, 30, WINWIDTH, WINHEIGHT );
    if (g_shell_config) {
      g_shell_config->read(fluid_prefs, Fd_Tool_Store::USER);
      g_shell_config->update_settings_dialog();
      g_shell_config->rebuild_shell_menu();
    }
    g_layout_list.read(fluid_prefs, Fd_Tool_Store::USER);
    main_window->show(argc,argv);
    toggle_widgetbin_cb(0,0);
    toggle_codeview_cb(0,0);
    if (!c && openlast_button->value() && Fluid.history.full_path[0][0] && Fluid.args.autodoc_path.empty()) {
      // Open previous file when no file specified...
      Fluid.open_project_file(Fluid.history.full_path[0]);
    }
  }
  undo_suspend();
  if (c && !read_file(c,0)) {
    if (Fluid.batch_mode) {
      fprintf(stderr,"%s : %s\n", c, strerror(errno));
      exit(1);
    }
    fl_message("Can't read %s: %s", c, strerror(errno));
  }
  undo_resume();

  // command line args override code and header filenames from the project file
  // in batch mode only
  if (Fluid.batch_mode) {
    if (!Fluid.args.code_filename.empty()) {
      g_project.code_file_set = 1;
      g_project.code_file_name = Fluid.args.code_filename;
    }
    if (!Fluid.args.header_filename.empty()) {
      g_project.header_file_set = 1;
      g_project.header_file_name = Fluid.args.header_filename;
    }
  }

  if (Fluid.args.update) {            // fluid -u
    write_file(c,0);
    if (!Fluid.args.compile)
      exit(0);
  }

  if (Fluid.args.compile) {           // fluid -c[s]
    if (Fluid.args.strings)
      write_strings_cb(0,0);
    write_cb(0,0);
    exit(0);
  }

  // don't lock up if inconsistent command line arguments were given
  if (Fluid.batch_mode)
    exit(0);

#ifdef WIN32
  Fl::set_font(FL_COURIER, " Consolas");
  Fl::set_font(FL_COURIER_BOLD, "BConsolas");
  Fl::set_font(FL_COURIER_ITALIC, "IConsolas");
  Fl::set_font(FL_COURIER_BOLD_ITALIC, "PConsolas");
#endif
  Fluid.project().set_modflag(0);
  undo_clear();
#ifndef _WIN32
  signal(SIGINT,sigint);
#endif

  // Set (but do not start) timer callback for external editor updates
  ExternalCodeEditor::set_update_timer_callback(external_editor_timer);

#ifndef NDEBUG
  // check if the user wants FLUID to generate image for the user documentation
  if (!Fluid.args.autodoc_path.empty()) {
    run_autodoc(Fluid.args.autodoc_path);
    return 0;
  }
#endif

#ifdef _WIN32
  Fl::run();
#else
  while (!quit_flag) Fl::wait();
  if (quit_flag) exit_cb(0,0);
#endif // _WIN32

  undo_clear();
  return (0);
}

/// \}

