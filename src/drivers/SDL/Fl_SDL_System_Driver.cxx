//
// Definition of SDL system driver for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2022 by Bill Spitzak and others.
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

#include <config.h>
#include <FL/platform.H>
#include "Fl_SDL_System_Driver.H"
#include <FL/Fl.H>

#include "../../src/Fl_Timeout.h"


/*
 Creates a driver that manages all system related calls.
 This function must be implemented once for every platform.
 */
Fl_System_Driver *Fl_System_Driver::newSystemDriver() {
  return new Fl_SDL_System_Driver();
}

Fl_SDL_System_Driver::Fl_SDL_System_Driver()
: Fl_System_Driver()
{
  printf("SDL System Driver created\n");
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    printf("error initializing SDL: %s\n", SDL_GetError());
  }
}

Fl_SDL_System_Driver::~Fl_SDL_System_Driver()
{
  // This is never called:
  printf("SDL System Driver destroyed\n");
  SDL_Quit();
}

static void handleKeyboardEvent(SDL_Event &e)
{
//  fl_lock_function();
//  Fl::compose_state = 0;
//  handled = Fl::handle(FL_KEYBOARD, w);
//  fl_unlock_function();
//  return handled;

  if (e.type==SDL_KEYDOWN) {
    Fl::e_number = FL_KEYDOWN;
  } else if (e.type==SDL_KEYUP) {
    Fl::e_number = FL_KEYUP;
  }
  Fl::e_keysym = 0;
  static char text[10];
  switch (e.key.keysym.scancode) {
    case SDL_SCANCODE_ESCAPE:
//  switch (e.key.keysym.sym) {
//    case SDLK_ESCAPE:
      Fl::e_original_keysym = Fl::e_keysym = FL_Escape;
      Fl::e_text = text; text[0] = 0x1f; text[1] = 0;
      Fl::e_length = 1;
      Fl::e_state = 0;
      break;
    default:
      break;
  }
  if (Fl::e_keysym)
    Fl::handle(Fl::e_number, Fl_X::first->w);

  //  Fl::e_keysym = Fl::e_original_keysym = sym;
//  /*NSLog(@"cocoaKeyboardHandler: keycode=%08x keysym=%08x mods=%08x symbol=%@ (%@)",
//   keyCode, sym, mods, [theEvent characters], [theEvent charactersIgnoringModifiers]);*/
//  // If there is text associated with this key, it will be filled in later.
//  Fl::e_length = 0;
//  Fl::e_text = (char*)"";
}

double Fl_SDL_System_Driver::wait(double time_to_wait) {

  int have_message = 0;

  if (Fl::idle || Fl::damage())
    time_to_wait = 0.0;

  // if there are no more windows and this timer is set
  // to FOREVER, continue through or look up indefinitely
  if (!Fl::first_window() && time_to_wait == 1e20)
    time_to_wait = 0.0;

//  fl_unlock_function();

  time_to_wait = (time_to_wait > 10000 ? 10000 : time_to_wait);

  time_to_wait = Fl_Timeout::time_to_wait(time_to_wait);

  SDL_Event event;
  int t_msec = (int)(time_to_wait * 1000.0 + 0.5);
  SDL_WaitEventTimeout(&event, t_msec);

//  fl_lock_function();

  for (;;) {
//    if (fl_send_system_handlers(&event))
//      continue;
    switch (event.type) {
      case SDL_KEYDOWN:
      case SDL_KEYUP:
        handleKeyboardEvent(event);
    }
    if (SDL_PollEvent(&event)!=1)
      break;
  }

  Fl::flush();

  // This should return 0 if only timer events were handled:
  return 1;
}
