//
// "$Id$"
//
// Hello, World! program for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2010 by Bill Spitzak and others.
//
// This library is free software. Distribution and use rights are outlined in
// the file "COPYING" which should have been included with this file.  If this
// file is missing or damaged, see the license at:
//
//     http://www.fltk.org/COPYING.php
//
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>

#include <FL/Fl_Preferences.H>

int main(int argc, char **argv) {
#if 0
  Fl_Window *window = new Fl_Window(340,180);
  Fl_Box *box = new Fl_Box(20,40,300,100,"Hello, World!");
  box->box(FL_UP_BOX);
  box->labelfont(FL_BOLD+FL_ITALIC);
  box->labelsize(36);
  box->labeltype(FL_SHADOW_LABEL);
  window->end();
  window->show(argc, argv);
  return Fl::run();
#else
    Fl_Preferences prefs("/Users/matt/test.xml", 0L, 0L, Fl_Preferences::FORMAT_XML);
    Fl_Preferences car(prefs, "car");
    car.set("color", "red");
    car.set("seats", 4);
    Fl_Preferences audio(car, "audio");
    audio.set("volume", 11);
    audio.addInline("Sennheiser");
    Fl_Preferences wheel1 = car.addGroup("wheel");
    wheel1.set("position", "front left");
    wheel1 = car.addGroup("wheel");
    wheel1.set("position", "front right");
    prefs.flush(1);

    int n = audio.groups();
    for (int i=0; i<n; i++) {
        Fl_Preferences inCar(audio, i);
        printf("%d %s\n", i, inCar.name());
        const char *text = audio.getInline(i);
        printf("%d %s\n", i, text);
    }
#endif
}

//
// End of "$Id$".
//

