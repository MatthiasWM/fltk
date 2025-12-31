//
// Button/callback test program for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
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

#include <stdlib.h>
#include <stdio.h>
#include <fltk3/Fl.H>
#include <fltk3/Fl_Window.H>
#include <fltk3/Button.H>
#include <fltk3/fl_ask.H>

#include <FL/Fl_Button.H>

// TODO: Fl_Widget
void beepcb(fltk3::Widget *, void *) {
  fl_beep();
  fflush(stdout);
}

void exitcb(fltk3::Widget *, void *) {
  exit(0);
}

#if 0
// test Fl::add_fd()...
void stdin_cb(int, void*) {
  char buf[1000];
  fgets(buf, sizeof(buf), stdin);
  printf("stdin callback\n");
}
#endif

int main(int argc, char ** argv) {
  Fl::args(argc, argv);
  Fl_Window *window = new Fl_Window(320,65);
  auto *b1 = new Fl_Button(20, 20, 80, 25, "&Beep");
  b1->callback(beepcb,0);
  b1->clear_button();
  /*fltk3::Button *b2 =*/ new fltk3::Button(120,20, 80, 25, "&no op");
  fltk3::Button *b3 = new fltk3::Button(220,20, 80, 25, "E&xit");
  b3->callback(exitcb,0);
  window->end();
  window->show(argc,argv);
#if 0
  Fl::add_fd(0, stdin_cb);
#endif
  return Fl::run();
}
