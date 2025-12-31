//
// Event and other names header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2025 by Bill Spitzak and others.
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

// Thanks to Greg Ercolano for this addition.

/**
 \file names.h
 This file defines arrays of human readable names for FLTK symbolic constants.
*/

#ifndef FL_NAMES_H
#define FL_NAMES_H

#include <fltk3/Fl.H>  // for event constants
#include <string>
#include <map>

/** \defgroup fl_events Events handling functions
    @{
 */

/**
  This is an array of event names you can use to convert event numbers into names.

  The array gets defined inline wherever your '\#include <fltk3/names.h>' appears.

  \b Example:
  \code
  #include <fltk3/names.h>         // array will be defined here
  int MyClass::handle(int e) {
      printf("Event was %s (%d)\n", fl_eventnames[e], e);
      // ..resulting output might be e.g. "Event was fltk3::PUSH (1)"..
      [..]
  }
  \endcode
 */
static std::map<int, const char*> fl_eventnames = {
  { fltk3::NO_EVENT, "fltk3::NO_EVENT" },
  { fltk3::PUSH, "fltk3::PUSH" },
  { fltk3::RELEASE, "fltk3::RELEASE" },
  { fltk3::ENTER, "fltk3::ENTER" },
  { fltk3::LEAVE, "fltk3::LEAVE" },
  { fltk3::DRAG, "fltk3::DRAG" },
  { fltk3::FOCUS, "fltk3::FOCUS" },
  { fltk3::UNFOCUS, "fltk3::UNFOCUS" },
  { fltk3::KEYDOWN, "fltk3::KEYDOWN" },
  { fltk3::KEYUP, "fltk3::KEYUP" },
  { fltk3::CLOSE, "fltk3::CLOSE" },
  { fltk3::MOVE, "fltk3::MOVE" },
  { fltk3::SHORTCUT, "fltk3::SHORTCUT" },
  { fltk3::DEACTIVATE, "fltk3::DEACTIVATE" },
  { fltk3::ACTIVATE, "fltk3::ACTIVATE" },
  { fltk3::HIDE, "fltk3::HIDE" },
  { fltk3::SHOW, "fltk3::SHOW" },
  { fltk3::PASTE, "fltk3::PASTE" },
  { fltk3::SELECTIONCLEAR, "fltk3::SELECTIONCLEAR" },
  { fltk3::MOUSEWHEEL, "fltk3::MOUSEWHEEL" },
  { fltk3::DND_ENTER, "fltk3::DND_ENTER" },
  { fltk3::DND_DRAG, "fltk3::DND_DRAG" },
  { fltk3::DND_LEAVE, "fltk3::DND_LEAVE" },
  { fltk3::DND_RELEASE, "fltk3::DND_RELEASE" },
  { fltk3::SCREEN_CONFIGURATION_CHANGED, "fltk3::SCREEN_CONFIGURATION_CHANGED" },
  { fltk3::FULLSCREEN, "fltk3::FULLSCREEN" },
  { fltk3::ZOOM_GESTURE, "fltk3::ZOOM_GESTURE" },
  { fltk3::ZOOM_EVENT, "fltk3::ZOOM_EVENT" },
  { fltk3::BEFORE_TOOLTIP, "fltk3::BEFORE_TOOLTIP" },
  { fltk3::BEFORE_MENU, "fltk3::BEFORE_MENU" },
  { fltk3::APP_ACTIVATE, "fltk3::APP_ACTIVATE" },
  { fltk3::APP_DEACTIVATE, "fltk3::APP_DEACTIVATE" },
  { /*fltk3::EVENT_*/ 30, "fltk3::EVENT_30" }, // not yet defined, just in case it /will/ be defined ...
  { /*fltk3::EVENT_*/ 31, "fltk3::EVENT_31" }, // not yet defined, just in case it /will/ be defined ...
  { /*fltk3::EVENT_*/ 32, "fltk3::EVENT_32" },  // not yet defined, just in case it /will/ be defined ...
  { Fl::Pen::DETECTED, "Fl::Pen::DETECTED" },
  { Fl::Pen::CHANGED, "Fl::Pen::CHANGED" },
  { Fl::Pen::ENTER, "Fl::Pen::ENTER" },
  { Fl::Pen::LEAVE, "Fl::Pen::LEAVE" },
  { Fl::Pen::TOUCH, "Fl::Pen::TOUCH" },
  { Fl::Pen::LIFT, "Fl::Pen::LIFT" },
  { Fl::Pen::HOVER, "Fl::Pen::HOVER" },
  { Fl::Pen::DRAW, "Fl::Pen::DRAW" },
  { Fl::Pen::BUTTON_PUSH, "Fl::Pen::BUTTON_PUSH" },
  { Fl::Pen::BUTTON_RELEASE, "Fl::Pen::BUTTON_RELEASE" }
};

/**
 Return the C++ symbol of an fltk3::Event as a string.
 \param[in] event index as an integer
 \return C++ symbol of event index as a string
 */
inline std::string fl_eventname_str(int event) {
  auto it = fl_eventnames.find(event);
  if (it == fl_eventnames.end()) {
    return "fltk3::EVENT_" + std::to_string(event);
  } else {
    return it->second;
  }
}


/**
  This is an array of font names you can use to convert font numbers into names.

  The array gets defined inline wherever your '\#include <fltk3/names.h>' appears.

  \b Example:
  \code
  #include <fltk3/names.h>         // array will be defined here
  int MyClass::my_callback(fltk3::Widget *w, void*) {
      int fnum = w->labelfont();
      // Resulting output might be e.g. "Label's font is FL_HELVETICA (0)"
      printf("Label's font is %s (%d)\n", fl_fontnames[fnum], fnum);
      // ..resulting output might be e.g. "Label's font is FL_HELVETICA (0)"..
      [..]
  }
  \endcode
 */
const char * const fl_fontnames[] =
{
  "FL_HELVETICA",
  "FL_HELVETICA_BOLD",
  "FL_HELVETICA_ITALIC",
  "FL_HELVETICA_BOLD_ITALIC",
  "FL_COURIER",
  "FL_COURIER_BOLD",
  "FL_COURIER_ITALIC",
  "FL_COURIER_BOLD_ITALIC",
  "FL_TIMES",
  "FL_TIMES_BOLD",
  "FL_TIMES_ITALIC",
  "FL_TIMES_BOLD_ITALIC",
  "FL_SYMBOL",
  "FL_SCREEN",
  "FL_SCREEN_BOLD",
  "FL_ZAPF_DINGBATS",
};

/**
  Return the C++ symbol of an Fl_Font as a string.
  \param[in] font index as an integer
  \return C++ symbol of font index as a string
*/
inline std::string fl_fontname_str(int font) {
  if ((font < 0) || (font >= FL_ZAPF_DINGBATS)) {
    return "FL_FONT_" + std::to_string(font);
  } else {
    return fl_fontnames[font];
  }
}


/**
 This is an array of callback reason names you can use to convert callback reasons into names.

 The array gets defined inline wherever your '\#include <fltk3/names.h>' appears.
 */
const char * const fl_callback_reason_names[] =
{
  "FL_REASON_UNKNOWN",
  "FL_REASON_SELECTED",
  "FL_REASON_DESELECTED",
  "FL_REASON_RESELECTED",
  "FL_REASON_OPENED",
  "FL_REASON_CLOSED",
  "FL_REASON_DRAGGED",
  "FL_REASON_CANCELLED",
  "FL_REASON_CHANGED",
  "FL_REASON_GOT_FOCUS",
  "FL_REASON_LOST_FOCUS",
  "FL_REASON_RELEASED",
  "FL_REASON_ENTER_KEY",
  NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  "FL_REASON_USER", "FL_REASON_USER+1", "FL_REASON_USER+2", "FL_REASON_USER+3",
};

/**
  Return the C++ symbol of an Fl_Callback_Reason as a string.
  \param[in] reason as an integer
  \return C++ symbol of reason as a string
*/
inline std::string fl_callback_reason_str(int reason) {
  if ((reason < 0) || (reason >= FL_REASON_USER+3) || (fl_callback_reason_names[reason] == nullptr)) {
    return "FL_REASON_" + std::to_string(reason);
  } else {
    return fl_callback_reason_names[reason];
  }
}

/** @} */

#endif /* FL_NAMES_H */
