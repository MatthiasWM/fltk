//
// Slider header file for the Fast Light Tool Kit (FLTK).
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

/* \file
   Fl_Slider widget . */

#ifndef Fl_Slider_H
#define Fl_Slider_H

#ifndef Fl_Valuator_H
#include "Fl_Valuator.H"
#endif

// values for type(), lowest bit indicate horizontal:
#define FL_VERT_SLIDER          0
#define FL_HOR_SLIDER           1
#define FL_VERT_FILL_SLIDER     2
#define FL_HOR_FILL_SLIDER      3
#define FL_VERT_NICE_SLIDER     4
#define FL_HOR_NICE_SLIDER      5

/**
  The Fl_Slider widget contains a sliding knob inside a box. It is
  often used as a scrollbar.  Moving the box all the way to the
  top/left sets it to the minimum(), and to the bottom/right to the
  maximum().  The minimum() may be greater than the maximum() to
  reverse the slider direction.

  Use void Fl_Widget::type(int) to set how the slider is drawn,
  which can be one of the following:

  \li FL_VERTICAL - Draws a vertical slider (this is the default).
  \li FL_HORIZONTAL - Draws a horizontal slider.
  \li FL_VERT_FILL_SLIDER - Draws a filled vertical slider,
      useful as a progress or value meter.
  \li FL_HOR_FILL_SLIDER - Draws a filled horizontal  slider,
      useful as a progress or value meter.
  \li FL_VERT_NICE_SLIDER - Draws a vertical slider with  a nice
      looking control knob.
  \li FL_HOR_NICE_SLIDER - Draws a horizontal slider with  a
      nice looking control knob.

  Logarithmic scaling can be enabled using log(bool) to make the slider
  use a logarithmic rather than linear scale. This is useful for audio
  controls and other parameters that span several orders of magnitude.

  Tick marks can be drawn on Nice_Slider types by setting tick_size()
  to a non-zero value. This is particularly useful for visual feedback
  on the slider range.

  \image html  slider.png
  \image latex slider.png "Fl_Slider" width=4cm
*/
class FL_EXPORT Fl_Slider : public Fl_Valuator {

  float slider_size_;
  uchar slider_;
  int tick_size_;
  uchar log_;
  void _Fl_Slider();
  void draw_bg(int, int, int, int);

protected:

  // these allow subclasses to put the slider in a smaller area:
  void draw(int, int, int, int);
  int handle(int, int, int, int, int);
  void draw() override;

  /** Convert linear position to value, applying log scale if enabled */
  double position_value(double p, double p1, double p2) const;
  /** Convert value to linear position, applying log scale if enabled */
  double value_position(double v, double p1, double p2) const;

public:

  int handle(int) override;
  Fl_Slider(int X,int Y,int W,int H, const char *L = 0);
  Fl_Slider(uchar t,int X,int Y,int W,int H, const char *L);

  int scrollvalue(int pos,int size,int first,int total);
  void bounds(double a, double b);

  /**
    Get the dimensions of the moving piece of slider.
  */
  float slider_size() const {return slider_size_;}

  /**
    Set the dimensions of the moving piece of slider. This is
    the fraction of the size of the entire widget. If you set this
    to 1 then the slider cannot move.  The default value is .08.

    For the "fill" sliders this is the size of the area around the
    end that causes a drag effect rather than causing the slider to
    jump to the mouse.
  */
  void slider_size(double v);

  /** Gets the slider box type. */
  Fl_Boxtype slider() const {return (Fl_Boxtype)slider_;}

  /** Sets the slider box type. */
  void slider(Fl_Boxtype c) {slider_ = (uchar)c;}

  /**
    Gets the tick mark size in pixels.
    The default value is 0, which disables tick marks.
    Tick marks are only drawn for Nice_Slider types.
  */
  int tick_size() const {return tick_size_;}

  /**
    Sets the tick mark size in pixels.
    Set to 0 to disable tick marks (the default).
    Tick marks are drawn on Nice_Slider types when tick_size is > 0.
    A typical value is 4 pixels.
    \param[in] n tick mark size in pixels (0 to disable)
  */
  void tick_size(int n);

  /**
    Returns whether logarithmic scaling is enabled.
    When log scaling is enabled, the slider uses a logarithmic
    rather than linear mapping between position and value.
  */
  bool log() const {return log_ != 0;}

  /**
    Enables or disables logarithmic scaling.
    When enabled, the slider uses a logarithmic mapping between
    position and value. This is useful for controls like volume
    sliders where the perceptual response is logarithmic.

    Note: For logarithmic scaling to work correctly, both minimum()
    and maximum() must be positive and non-zero.

    \param[in] v true to enable logarithmic scaling, false for linear
  */
  void log(bool v);
};

#endif
