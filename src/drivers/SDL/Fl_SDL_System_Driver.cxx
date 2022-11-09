
/*
 Notes on macOS drivers:

 There is no Fl_Cocoa_Screendriver::flush(). Instead we flush in Fl_Cocoa_Window_Driver::q_release_context();

 Fl_Timeout::time_to_wait(time_to_wait) is called twice in every loop
 - First in Fl_Darwin_System_Driver:786, Fl_Darwin_System_Driver::wait(double)
 - Second in Fl_Darwin_System_Driver:759, do_queued_events( double time = 0.0 )


 */

#include "Fl_SDL_System_Driver.H"

#include <config.h>
#include <FL/Fl.H>
#include <FL/platform.H>
#include "Fl_SDL_Window_Driver.H"
#include "Fl_SDL_Screen_Driver.H"
#include "src/Fl_Timeout.h"

Fl_SDL_System_Driver::Fl_SDL_System_Driver()
{
}

Fl_SDL_System_Driver::~Fl_SDL_System_Driver()
{
}

double Fl_SDL_System_Driver::wait(double time_to_wait)
{
  // handle pending timers and events
  // fix the time to wait to accomodate the next timer
  time_to_wait = Fl_System_Driver::wait(time_to_wait);

  // flush all pending graphics calls
  Fl::flush();

  if (Fl::idle) // 'idle' may have been set within flush()
    time_to_wait = 0.0;

  int got_events = 0;

  // Check for re-entrant condition
//  if ( dataready.IsThreadRunning() ) {
//    dataready.CancelThread(DEBUGTEXT("AVOID REENTRY\n"));
//  }

  // Start thread to watch for data ready
//  if ( dataready.GetNfds() ) {
//    dataready.StartThread();
//  }

  // Elapse timeouts and calculate waiting time
  Fl_Timeout::elapse_timeouts();
  // TODO: SDL: Didn't we just call that in Fl_System_Driver::wait?
  time_to_wait = Fl_Timeout::time_to_wait(time_to_wait);

  //fl_unlock_function();
  Fl::unlock();
  SDL_Event event;
  while (SDL_WaitEventTimeout(&event, time_to_wait/1000)) {
    got_events = 1;
    // TODO: SDL: handle event
    time_to_wait = 0; // TODO: SDL: this would run the entire event loop again after handling just a single event
  }
//  fl_lock_function();
  Fl::lock();

  return got_events;


//  SDL_Event event;
//
//  bool done = false;
//  while((!done) && (SDL_WaitEvent(&event))) {
//    switch(event.type) {
//      case SDL_USEREVENT:
//        //        HandleUserEvents(&event);
//        break;
//
//      case SDL_KEYDOWN:
//        // Handle any key presses here.
//        break;
//
//      case SDL_MOUSEBUTTONDOWN:
//        // Handle mouse clicks here.
//        break;
//
//      case SDL_QUIT:
//        done = true;
//        break;
//
//      default:
//        break;
//    }   // End switch
//
//  }   // End while

  //  SDL_Delay(3000);

//  SDL_DestroyWindow(_screen);
//  SDL_Quit();
}
