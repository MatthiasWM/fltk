//
// Project Reader Class for Fast Light User Interface Designer (FLUID).
//
// Copyright 1998-2023 by Bill Spitzak and others.
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

#ifndef FLUID_STREAMS_PROJECT_READER_H
#define FLUID_STREAMS_PROJECT_READER_H

#include "Fl_Type.h"

#include "fluid.h"

#include <FL/fl_attr.h>

class Fl_Type;

extern int fdesign_flip;

namespace fluid {

namespace stream {

int read_file(const char *, int merge, Strategy strategy=kAddAsLastChild);

class ProjectReader
{
protected:
  /// Project input file
  FILE *fin;
  /// Number of most recently read line
  int lineno;
  /// Pointer to the file path and name (not copied!)
  const char *fname;
  /// Expanding buffer to store the most recently read word
  char *buffer;
  /// Exact size of the expanding buffer in bytes
  int buflen;

  void expand_buffer(int length);

  int nextchar() { for (;;) { int ret = fgetc(fin); if (ret!='\r') return ret; } }

public:
  /// Reference to the project that will be read.
  fluid::Project &project;
  /// Holds the file version number after reading the "version" tag
  double read_version;

public:
  ProjectReader(fluid::Project &project);
  ~ProjectReader();
  int open_read(const char *s);
  int close_read();
  const char *filename_name();
  int read_quoted();
  Fl_Type *read_children(Fl_Type *p, int merge, Strategy strategy, char skip_options=0);
  int read_project(const char *, int merge, Strategy strategy=kAddAsLastChild);
  void read_error(const char *format, ...);
  const char *read_word(int wantbrace = 0);
  int read_int();
  int read_fdesign_line(const char*& name, const char*& value);
  void read_fdesign();
};

} // namespace stream

} // namespace fluid

#endif // FLUID_STREAMS_PROJECT_READER_H
