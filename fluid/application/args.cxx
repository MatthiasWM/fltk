//
// Application Args Class for Fast Light User Interface Designer (FLUID).
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

#include "application/args.h"
#include "application/application.h"

#include <FL/fl_ask.H>

using namespace fluid;

application::Args::Args(Application &theApp)
: app(theApp) {}

/**
 Handle one command line argument.

 If an argument is not recognized, it is not supported and the function
 returns 0. If the argument is supported, the function returns the number
 of arguments used. The argument is then removed from the list.

 \param[in] argc number of arguments in the list
 \param[in] argv pointer to an array of arguments
 \param[inout] i current argument index
 \return number of arguments used; if 0, the argument is not supported
 */
int application::Args::arg(int argc, char** argv, int& i) {
  if (argv[i][0] != '-')
    return 0;
  if (argv[i][1] == 'd' && !argv[i][2]) {
    debug = true;
    i++; return 1;
  }
  if (argv[i][1] == 'u' && !argv[i][2]) {
    update = true;
    app.batch_mode = true;
    i++; return 1;
  }
  if (argv[i][1] == 'c' && !argv[i][2]) {
    Fluid.args.compile = true;
    Fluid.batch_mode = true;
    i++; return 1;
  }
  if (argv[i][1] == 'c' && argv[i][2] == 's' && !argv[i][3]) {
    Fluid.args.compile = true;
    Fluid.args.strings = true;
    Fluid.batch_mode = true;
    i++; return 1;
  }
  if (argv[i][1] == 'o' && !argv[i][2] && i+1 < argc) {
    Fluid.args.code_filename = argv[i+1];
    Fluid.batch_mode = true;
    i += 2; return 2;
  }
#ifndef NDEBUG
  if ((i+1 < argc) && (strcmp(argv[i], "--autodoc") == 0)) {
    Fluid.args.autodoc_path = argv[i+1];
    i += 2; return 2;
  }
#endif
  if (argv[i][1] == 'h' && !argv[i][2]) {
    if ( (i+1 < argc) && (argv[i+1][0] != '-') ) {
      Fluid.args.header_filename = argv[i+1];
      Fluid.batch_mode = true;
      i += 2;
      return 2;
    } else {
      // a lone "-h" without a filename will output the help string
      return 0;
    }
  }
  return 0;
}

int application::Args::arg_callback(int argc, char** argv, int& i) {
    return Fluid.args.arg(argc, argv, i);
}

/**
 * Parses the command line arguments and sets the appropriate flags in the
 * fluid::App_Args object. If an unsupported argument is found, or if the
 * number of arguments is incorrect, prints an error message and returns false.
 * Otherwise, returns true.
 *
 * \param[in] argc Number of arguments in the list.
 * \param[in] argv Pointer to an array of arguments. 
 * \return -1 if there was an error in the command line
 *         or the index of the .fl project file
 */
int application::Args::read(int argc, char **argv) {
  Fl::args_to_utf8(argc, argv); // for MSYS2/MinGW
  int i = 1;
  if (   (Fl::args(argc, argv, i, arg_callback) == 0)     // unsupported argument found
      || (Fluid.batch_mode && (i != argc-1))        // .fl filename missing
      || (!Fluid.batch_mode && (i < argc-1))        // more than one filename found
      || (argv[i] && (argv[i][0] == '-'))) {  // unknown option
    static const char *msg =
      "usage: %s <switches> name.fl\n"
      " -u : update .fl file and exit (may be combined with '-c' or '-cs')\n"
      " -c : write .cxx and .h and exit\n"
      " -cs : write .cxx and .h and strings and exit\n"
      " -o <name> : .cxx output filename, or extension if <name> starts with '.'\n"
      " -h <name> : .h output filename, or extension if <name> starts with '.'\n"
      " -d : enable internal debugging\n";
    const char *app_name = NULL;
    if ( (argc > 0) && argv[0] && argv[0][0] )
      app_name = fl_filename_name(argv[0]);
    if ( !app_name || !app_name[0])
      app_name = "fluid";
#ifdef _MSC_VER
    fl_message(msg, app_name);
#else
    fprintf(stderr, msg, app_name);
#endif
    return -1;
  }
  return i;
}

