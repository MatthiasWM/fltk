//
// String Writer Class for Fast Light User Interface Designer (FLUID).
//
// Copyright 1998-2021 by Bill Spitzak and others.
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

#ifndef FLUID_STREAMS_STRING_WRITER_H
#define FLUID_STREAMS_STRING_WRITER_H

#include "../src/Fl_String.H"

namespace fluid {

namespace stream {

int write_strings(const Fl_String &filename);

} // namespace stream

} // namespace fluid

#endif // FLUID_STREAMS_STRING_WRITER_H
