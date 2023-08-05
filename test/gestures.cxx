//
// Multitouch gesture test program for the Fast Light Tool Kit (FLTK).
//
// Copyright 2022-2023 by Bill Spitzak and others.
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

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Dial.H>

#include <FL/fl_ask.H>
#include <FL/fl_draw.H>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

const float zmin = 0.4f; // min. zoom
const float zmax = 2.0f; // max. zoom

void exitcb(Fl_Widget *, void *) {
  exit(0);
}

class myButton : public Fl_Button {
  char txt_[20];
  double zoom;
  int bx, by, bw, bh;

public:
  myButton(int x, int y, int w, int h): Fl_Button(x, y, w, h, "") {
    bx = x; by = y; bw = w; bh = h;
    zoom = 1.0;
    strcpy(txt_, "1.0");
    align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE);
    label(txt_);
  }

  int handle(int event) FL_OVERRIDE {
    int ret = 0;
    double z = 0.0;
    switch(event) {
      case FL_MOUSEWHEEL:
      case FL_SCROLL_GESTURE:
        if (Fl::event_dy() > 0)
          zoom += 0.1;
        else
          zoom -= 0.1;
        if (zoom < zmin) zoom = zmin;
        if (zoom > zmax) zoom = zmax;
        sprintf(txt_, "%5.3f", zoom);
        resize(bx, by, int(bw * zoom), int(bh * zoom));
        parent()->redraw();
        ret = 1;
        break;
      case FL_ZOOM_GESTURE:
        z = Fl::event_value();
        printf("[gestures.cxx:%d] myButton::handle FL_ZOOM_GESTURE: %5.3f\n", __LINE__, z);
        fflush(stdout);
        if (z)
          zoom *= z;
        if (zoom < zmin) zoom = zmin;
        if (zoom > zmax) zoom = zmax;
        sprintf(txt_, "%5.3f", zoom);
        resize(bx, by, int(bw * zoom), int(bh * zoom));
        parent()->redraw();
        ret = 1;
        break;
      default:
        break;
    }
    return Fl_Button::handle(event) ? 1 : ret;
  }
};


class myDial : public Fl_Dial {
  char txt_[20];
  double angle;
  int bx, by, bw, bh;

public:
  myDial(int x, int y, int w, int h): Fl_Dial(x, y, w, h, "") {
    bx = x; by = y; bw = w; bh = h;
    angle = 90.0;
    strcpy(txt_, "90.0");
    align(FL_ALIGN_CENTER | FL_ALIGN_TOP);
    label(txt_);
    angles(0, 360);
    type(FL_LINE_DIAL);
    minimum(0);
    maximum(360);
    step(1);
    set_value();
  }

  void set_value() {
    value(double(int(angle + 180) % 360));
  }

  void set_label() {
    angle = (int(value()) + 180) % 360;
    sprintf(txt_, "%3.0f", angle);
    parent()->redraw();
  }

  int handle(int event) FL_OVERRIDE { // myDial::handle()
    int ret = 0;
    switch(event) {
      case FL_MOUSEWHEEL:
      case FL_SCROLL_GESTURE:
        if (Fl::event_dy() > 0)
          angle += 10.0;
        else
          angle -= 10.;
        if (angle < 0.) angle += 360.;
        if (angle > 360.) angle -= 360;
        sprintf(txt_, "%3.0f", angle);
        set_value();
        parent()->redraw();
        return 1;
        break;
      case FL_ROTATE_GESTURE:
        // positive Fl::event_value() is counter-clockwise, Fl_Dial is the opposite
        angle -= Fl::event_value();
        if (angle < 0.) angle += 360.;
        if (angle > 360.) angle -= 360;
        sprintf(txt_, "%3.0f", angle);
        set_value();
        parent()->redraw();
        return 1;
        break;
      default:
        break;
    }
    return Fl_Dial::handle(event) ? 1 : ret;
  }
};

void dial_cb(Fl_Widget *w, void *v) {
  myDial *d = (myDial *)w;
  d->set_label();
}

class myPad : public Fl_Group{
  double rx, ry, rscale, rtheta;

public:
  myPad(int x, int y, int w, int h, const char * l = 0 ): Fl_Group(x, y, w, h, l) {
    reset();
  }

  void reset() {
    rx = 0;
    ry = 0;
    rscale = 1;
    rtheta = 0.0;
  }

  int handle(int event) FL_OVERRIDE { // myPad::handle()
    int ret = 0;
    double z = 0;
    switch(event) {
      case FL_MOUSEWHEEL:
        if (Fl::event_dy() > 0)
          rscale += 0.1;
        else
          rscale -= 0.1;
        if (rscale < zmin) rscale = zmin;
        if (rscale > zmax) rscale = zmax;
        redraw();
        return 1;
        break;
      case FL_ZOOM_GESTURE:
        z = Fl::event_value(FL_ZOOM_VALUE);
        if (z)
          rscale *= z;
        if (rscale < zmin) rscale = zmin;
        if (rscale > zmax) rscale = zmax;
        printf("[gestures.cxx:%d] myPad::handle(FL_ZOOM_GESTURE): %5.3f, new zoom = %5.3f\n", __LINE__, z, rscale);
        fflush(stdout);
        redraw();
        return 1;
        break;
      case FL_SCROLL_GESTURE: {
        int dx = int(Fl::event_value(FL_GESTURE_X));
        int dy = int(Fl::event_value(FL_GESTURE_Y));
        printf("[gestures.cxx:%d] myPad::handle(FL_SCROLL_GESTURE): (dx,dy) = (%2d, %2d), (%7.2f, %7.2f)\n", __LINE__,
               dx, dy, Fl::event_value(FL_GESTURE_X), Fl::event_value(FL_GESTURE_Y));
        fflush(stdout);
        const int max_x = w()/2 - 10;
        const int max_y = h()/2 - 10;
        const int min_x = - (w()/2 - 10);
        const int min_y = - (h()/2 - 10);
        rx += dx;
        ry += dy;
        if (rx < min_x ) rx = min_x;
        if (rx > max_x ) rx = max_x;
        if (ry < min_y) ry = min_y;
        if (ry > max_y) ry = max_y;
        redraw();
        return 1;
      }
      case FL_ROTATE_GESTURE: {
        double angle = Fl::event_value(); // use default index (0)
        rtheta += angle;
        if (rtheta < 0.) rtheta += 360.;
        if (rtheta > 360.) rtheta -= 360;
        printf("[gestures.cxx:%d] myPad::handle(FL_ROTATE_GESTURE): %7.3f° -> new angle = %7.3f°\n", __LINE__,
               angle, rtheta);
        fflush(stdout);
        redraw();
        return 1;
      }
      default:
        break;
    }
    return Fl_Group::handle(event) ? 1 : ret;

  } // myPad::handle()

  void draw() FL_OVERRIDE { // myPad::draw()
    Fl_Group::draw();

    int tx = x() + w() / 2;   // center of box
    int ty = y() + h() / 2;   // dto.

    fl_color(FL_BLACK);

    fl_push_matrix();
    fl_translate( tx + rx, ty + ry ); // center + rel. pos.
    fl_rotate( rtheta );
    fl_scale( rscale );

    fl_begin_polygon();
#if (0)                       // draw a square
    int rw = 30;              // width of square
    fl_vertex( -rw, -rw );    // top left
    fl_vertex( rw, -rw );     // top right
    fl_vertex( rw, rw );      // bottom right
    fl_vertex( -rw, rw );     // bottom left
#else                         // draw a triangle
    int rh = 50;
    int rw = 20;
    fl_vertex( 0, -rh );      // top
    fl_vertex( rw, rw );      // bottom right
    fl_vertex( -rw, rw );     // bottom left
#endif
    fl_end_polygon();

    fl_pop_matrix();

  } // myPad::draw()
};

int main(int argc, char **argv) {

  Fl_Double_Window *window = new Fl_Double_Window(3*440, 510);

  // embed widgets in their own groups to speed up drawing

  Fl_Group *g1 = new Fl_Group(0, 0, 440, 440);
  g1->box(FL_FLAT_BOX);
  g1->color(0xddffdd00); // light green
  /* myButton *b1 = */ new myButton(20, 20, 200, 100); // b1: [-Wunused-variable]
  myDial *d1 = new myDial(130, 240, 180, 180);
  d1->callback(dial_cb);
  g1->end();

  Fl_Group *g2 = new Fl_Group(440, 0, 440, 440);
  g2->box(FL_FLAT_BOX);
  g2->color(0xddddff00); // light blue
  myDial *d2 = new myDial(460, 30, 380, 380);
  d2->callback(dial_cb);
  g2->end();

  myPad *g3 = new myPad(880, 0, 440, 440);
  g3->box(FL_FLAT_BOX);
  g3->color(0xffdddd00); // light red
  g3->end();

  Fl_Button *b3 = new Fl_Button(10, 450, 1300, 50, "E&xit");
  b3->callback(exitcb, 0);

  window->end();
  window->resizable(window);
  window->size_range(430, 240);
  window->show(argc, argv);

  return Fl::run();
}
