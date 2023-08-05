//
// Windows touch gesture support code for the Fast Light Tool Kit (FLTK).
//
// Copyright 2021-2023 by Bill Spitzak and others.
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

// This file contains Windows specific code to support multitouch gestures
// and is compiled only on Windows platforms.

#if !defined(FL_DOXYGEN)

// We require Windows 7 features to support touch gestures
#define FL_WINVER_MIN 0x0601 // Windows 7 for touch gestures

#if !defined(WINVER) || (WINVER < FL_WINVER_MIN)
#ifdef WINVER
#undef WINVER
#endif
#define WINVER FL_WINVER_MIN
#endif
#if !defined(_WIN32_WINNT) || (_WIN32_WINNT < FL_WINVER_MIN)
#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif
#define _WIN32_WINNT FL_WINVER_MIN
#endif

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <math.h>
#include <windows.h>

// Debug flag:  0 = off, 1 = decode, 2 = zoom, 4 = pan, 8 = rotate,     // 0x0f
//              16 = two-finger tap, 32 = press and tap, 64 = unknown,  // 0x70
//              0x100 = handle(FL_ZOOM_GESTURE),                        // event handling
//              0x200 = handle(FL_SCROLL_GESTURE),
//              0x400 = handle(FL_ROTATE_GESTURE),
//              0xf00 = handle(<all gestures>),
//              0xfff = everything

#define DEBUG_TOUCH 0x0

// USE_TOUCH_DEFAULT: 1 = enable "all" default gestures, 0 = selected gestures
// The former enables all *default* gestures but not all special options.
// Special options can be enabled by extra bits in the GESTURECONFIG struct.
// See docs of SetGestureConfig(), struct GESTURECONFIG, and CloseGestureInfoHandle():
// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setgestureconfig
// https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-gestureinfo
// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-closegestureinfohandle
// Note: links as of Aug 9, 2023

#ifndef USE_TOUCH_DEFAULT
#define USE_TOUCH_DEFAULT (0)  // should be 0 unless for testing
#endif

#if DEBUG_TOUCH
#include <stdio.h> // DEBUG: printf()
#endif

/*
  The following table lists the various identifiers for multitouch gestures.

    Name               Value  Description
    GID_BEGIN            1    A gesture is starting.
    GID_END              2    A gesture is ending.
    GID_ZOOM             3    The zoom gesture.
    GID_PAN              4    The pan gesture.
    GID_ROTATE           5    The rotation gesture.
    GID_TWOFINGERTAP     6    The two-finger tap gesture.
    GID_PRESSANDTAP      7    The press and tap gesture.
*/
#if (DEBUG_TOUCH & 1)
static const char *gestures[] = {
  "unknown gesture", "GID_BEGIN",  "GID_END",          "GID_ZOOM",
  "GID_PAN",         "GID_ROTATE", "GID_TWOFINGERTAP", "GID_PRESSANDTAP"
};
#endif

// Handle "pan" gesture (FL_SCROLL_GESTURE):
//
// This function handles a system "pan" or two-finger scroll gesture.
// Hopefully this can be made cross-platform so we can use it on all
// platforms for an identical two-finger scroll experience.
//
// (1) Send FL_SCROLL_GESTURE.
// (2) Send FL_MOUSEWHEEL if handle(FL_SCROLL_GESTURE) returns 0.
//
// Note: it is important to send X and Y scrolling in two distinct FLTK
// events because X scrolling might be used by a horizontal scrollbar
// and Y scrolling might be used by a vertical scrollbar.
// BTW: The same is true for FL_MOUSEWHEEL events (the fallback).

static int handle_scroll(Fl_Window *window, int dx, int dy) {

  int ret_x = 0;
  int ret_y = 0;

  // handle dx first

  if (dx != 0) {

    Fl::e_value[0] = dx;
    Fl::e_value[1] = 0;

#if (DEBUG_TOUCH & 0xf00)
  printf("*** handle(FL_SCROLL_GESTURE) [%5.3f, %5.3f]\n",
         Fl::e_value[0], Fl::e_value[1]);
#endif

    ret_x = Fl::handle(FL_SCROLL_GESTURE, window);
    if (!ret_x) {
      Fl::e_dx = -dx;
      Fl::e_dy = 0;
#if (DEBUG_TOUCH & 0xf00)
      printf("--- handle(FL_MOUSEWHEEL) [%d, %d]\n",
            Fl::e_dx, Fl::e_dy);
#endif
      ret_x = Fl::handle(FL_MOUSEWHEEL, window);
    }
  }

  if (dy == 0)
    return ret_x;

  // now handle dy

  Fl::e_value[0] = 0;
  Fl::e_value[1] = dy;

#if (DEBUG_TOUCH & 0xf00)
  printf("*** handle(FL_SCROLL_GESTURE) [%5.3f, %5.3f]\n",
         Fl::e_value[0], Fl::e_value[1]);
#endif

  ret_y = Fl::handle(FL_SCROLL_GESTURE, window);
  if (!ret_y) {
    Fl::e_dx = 0;
    Fl::e_dy = -dy;
#if (DEBUG_TOUCH & 0xf00)
    printf("--- handle(FL_MOUSEWHEEL) [%d, %d]\n",
          Fl::e_dx, Fl::e_dy);
#endif
    ret_y = Fl::handle(FL_MOUSEWHEEL, window);
  }

  return (ret_x || ret_y);

} // handle_scroll

// This function is used to select parts or all touch gesture messages.

int fl_win32_SetGestureConfig(HWND hWnd) {

#if USE_TOUCH_DEFAULT // enable all gestures with default options

  GESTURECONFIG gc[] = {
    {0, GC_ALLGESTURES, 0}
  };

#else // enable selected gestures and (potentially more than default) options

  GESTURECONFIG gc[] = {
    {GID_PAN,           GC_PAN,           0},
    {GID_ROTATE,        GC_ROTATE,        0},
    {GID_PRESSANDTAP,   GC_PRESSANDTAP,   0},
    {GID_TWOFINGERTAP,  GC_TWOFINGERTAP,  0},
    {GID_ZOOM,          GC_ZOOM,          0}
  };

#endif

  const unsigned int nconfigs = sizeof(gc) / sizeof(gc[0]);

  BOOL bResult = SetGestureConfig(
                    hWnd,                   // window handle
                    0,                      // reserved, must be 0
                    nconfigs,               // number of GESTURECONFIG structs
                    gc,                     // array of GESTURECONFIG structs
                    sizeof(GESTURECONFIG)); // structure size

  if (!bResult) {
#if (DEBUG_TOUCH & 1)
    DWORD dwErr = GetLastError();
    printf("Error calling SetGestureConfig(), dwErr = %ld.\n", dwErr);
    fflush(stdout);
#endif
    return 0;
  }

#if (DEBUG_TOUCH & 1)
    printf("--- SetGestureConfig() returned success.\n");
    fflush(stdout);
#endif
  return 1;
} // fl_win32_SetGestureConfig()


// This function does all the system specific work to decode a gesture message.
// It returns 1 if the gesture could be decoded and has been handled by FLTK,
// zero (0) otherwise.

int fl_win32_DecodeGesture(Fl_Window *window, LPARAM lParam) {

  static int begin = 0; // set to 1 when GID_BEGIN is seen, 0 when events are handled
  int handled = 0;      // set to 1 if we handled the event

  // Create and populate a structure to retrieve the extra message info.
  GESTUREINFO gi;
  ZeroMemory(&gi, sizeof(GESTUREINFO));
  gi.cbSize = sizeof(GESTUREINFO);

  // Retrieve the extra message info.
  BOOL bResult = GetGestureInfo((HGESTUREINFO)lParam, &gi);

  if (!bResult) {
#if (DEBUG_TOUCH & 1)
    DWORD dwErr = GetLastError();
    printf("Could not retrieve gesture info, error = %ld.\n", dwErr);
#endif
    return 0; // message not handled
  }

  // now interpret the gesture
#if (DEBUG_TOUCH & 1)
  const char *name = gestures[0];
  if (gi.dwID < 8) {
    name = gestures[gi.dwID];
  }
  printf("Gesture %-16s (%lu): flags=%02x, args=%lu, pos=(%4d,%4d)\n",
          name, gi.dwID, (int)gi.dwFlags,
          (unsigned long)gi.ullArguments, gi.ptsLocation.x, gi.ptsLocation.y);
#endif // DEBUG_TOUCH

  switch (gi.dwID) {

    case GID_BEGIN:
      begin = 1;
      break;

    case GID_END:
      begin = 0;
      break;

    // Windows 'zoom' (aka pinch) event

    case GID_ZOOM: {

      static long prev_distance = 0;  // distance when "previous" gesture was processed
      double zoom_factor = 0.0;       // relative zoom factor

      if (begin) {
        prev_distance = (long)gi.ullArguments;  // distance between points (fingers)
        begin = 0;
        handled = 1;
        break;
      }

      long new_distance = (long)gi.ullArguments;
      if (new_distance == prev_distance) {
        handled = 1;  // handled although we don't send it to the app
        break;        // skip event if distance didn't change
      }

      zoom_factor = (double)new_distance / prev_distance;

#if (DEBUG_TOUCH & 2)
        printf("    previous zoom distance = %8ld, current = %8ld -> rel. zoom factor = %6.3f\n",
                prev_distance, new_distance, zoom_factor);
#endif
      prev_distance = new_distance;

      // OK, handle event
      Fl::e_dy = int((zoom_factor - 1.0) * 1000); // FLTK 1.3.x backwards compatibiity (macOS)
      Fl::e_value[0] = zoom_factor;               // since FLTK 1.4.0
      Fl::handle(FL_ZOOM_GESTURE, window);
      handled = 1;
      break;
    } // GID_ZOOM

    // Windows 'pan' event. Can be sent for one or two (or more) fingers.

    // --------------------------------------------------------------------------------
    // Note: According to testing Windows sends WM_GESTURE/GID_PAN messages for both
    // "panning" with one or two (or more) fingers. These can be distinguished by
    // using the variable 'distance' below. If it is 0 (zero) it's only one finger,
    // otherwise it's more than one finger.
    // The value is the distance *between* the two points (fingers).
    // --------------------------------------------------------------------------------
    // Implementation:
    // (1) Only one finger:       potentially send FL_DRAG ? Currently ignored.
    // (2) Two or more fingers:   send FL_SCROLL_GESTURE
    // --------------------------------------------------------------------------------

    case GID_PAN: {

      static int prev_x = 0;                // last known x position
      static int prev_y = 0;                // last known x position

      int distance = (int)gi.ullArguments;  // distance between points (see above)
      int curr_x = gi.ptsLocation.x;        // current x position
      int curr_y = gi.ptsLocation.y;        // current y position
      if (begin) {
        prev_x = curr_x;
        prev_y = curr_y;
        begin = 0;
        handled = 1;
        break;
      }

#if (DEBUG_TOUCH & 4)
      printf("    pan gesture at (%4d, %4d), d = %4d, prev. x/y = (%4d, %4d)\n",
              curr_x, curr_y, distance, prev_x, prev_y);
#endif // (DEBUG_TOUCH & 4)

      if (distance == 0) { // only one finger: ignore for now
        // printf("*** 'pan' gesture with one finger ignored.\n");
        // fflush(stdout);
        break; // don't set 'handled' flag
      }

      // handle event unless neither X nor Y position changed
      int dx = curr_x - prev_x;
      int dy = curr_y - prev_y;
      // remember current (now: previous) position
      prev_x = curr_x;
      prev_y = curr_y;
      if ((dx == 0) && (dy == 0)) {
        handled = 1;
        break;
      }
      // OK, handle event

      handle_scroll(window, dx, dy);
      handled = 1;
      break;
    } // GID_PAN

    // Windows 'rotate' event. Send relative angle to application.

    case GID_ROTATE: {

      static double prev_angle  = 0.0;    // previous angle of "line between" points (fingers)
      static double start_angle = 0.0;    // starting angle of rotation
      double curr_angle = 0.0;
      double rotation = 0.0;

      if (gi.dwFlags & GF_BEGIN) {        // rotate gesture starts, angle = absolute value
        start_angle = GID_ROTATE_ANGLE_FROM_ARGUMENT(gi.ullArguments);
        start_angle = start_angle / (2 * 3.1415926535) * 360;
        prev_angle = start_angle;

#if (DEBUG_TOUCH & 8)
        printf("    Rotate begin: angle (abs) = %7.3f\n", prev_angle);
#endif // (DEBUG_TOUCH & 8)
        begin = 0;
        handled = 1;
        break;
      }

      else { // not the first gesture event, angle = *RELATIVE* to start angle

        curr_angle = GID_ROTATE_ANGLE_FROM_ARGUMENT(gi.ullArguments);
        curr_angle = curr_angle / (2 * 3.1415926535) * 360;
        curr_angle = start_angle + curr_angle;
        rotation = curr_angle - prev_angle;

        if (fabs(rotation) < 0.01) { // delta too small, don't send to app (should not happen)
          handled = 1;               // ... but report that we handled it
          break;
        }

#if (DEBUG_TOUCH & 8)
        printf("    angle %7.3f, previous = %7.3f -> rotation = %7.3f\n", curr_angle, prev_angle, rotation);
#endif // (DEBUG_TOUCH & 8)

        prev_angle = curr_angle;
      }

      // OK, handle event
      Fl::e_value[0] = rotation;
      Fl::handle(FL_ROTATE_GESTURE, window);
      handled = 1;
      break;
    } // GID_ROTATE

    case GID_TWOFINGERTAP: {
      // Note: on macOS and other OS'es / devices (touchpad) this would be
      //       interpreted as a right button mouse click.
      // Not (yet?) implemented.
#if (DEBUG_TOUCH & 16)
      printf("    two finger tap\n");
#endif
      break;
    } // GID_TWOFINGERTAP

    case GID_PRESSANDTAP: {
      // Code for roll over goes here
      // Tested:
      // (1) press and hold one finger on the touch screen or device
      // (2) tap with another finger
      // (3) optionally move the (first/remaining) finger around
      // (4) release the finger
#if (DEBUG_TOUCH & 32)
      printf("    press and tap (aka roll over)\n");
#endif
      break;
    } // GID_PRESSANDTAP

    default: {
      // unknown gesture
#if (DEBUG_TOUCH & 64)
      printf("    unknown gesture (%ld)\n", (long)gi.dwID);
#endif
      break;
    }
  } // switch (gi.dwID)

  // NOTE: According to docs of CloseGestureInfoHandle() we must "close the handle"
  // if we handled the message to avoid memory leaks (but not if we didn't handle it).

  if (handled) {
#if (DEBUG_TOUCH & 1)
    printf("WM_GESTURE was handled. Closing handle now ...");
    fflush(stdout);
    bResult = CloseGestureInfoHandle((HGESTUREINFO)lParam);
    printf(" done (%s).\n", bResult ? "success" : "*FAIL*");
#else
    /* bResult = */ CloseGestureInfoHandle((HGESTUREINFO)lParam);
#endif
  }

#if DEBUG_TOUCH
  fflush(stdout);
#endif // DEBUG_TOUCH

  return handled;

} // fl_win32_DecodeGesture()

#endif // !defined(FL_DOXYGEN)
