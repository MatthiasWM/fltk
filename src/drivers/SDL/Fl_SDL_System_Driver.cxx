
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


/*
 * convert the current mouse chord into the FLTK modifier state
 */
//static unsigned int mods_to_e_state( NSUInteger mods )
//{
//  long state = 0;
//  if ( mods & NSCommandKeyMask ) state |= FL_META;
//  if ( mods & NSAlternateKeyMask ) state |= FL_ALT;
//  if ( mods & NSControlKeyMask ) state |= FL_CTRL;
//  if ( mods & NSShiftKeyMask ) state |= FL_SHIFT;
//  if ( mods & NSAlphaShiftKeyMask ) state |= FL_CAPS_LOCK;
//  unsigned int ret = ( Fl::e_state & 0xff000000 ) | state;
//  Fl::e_state = ret;
//  //printf( "State 0x%08x (%04x)\n", Fl::e_state, mods );
//  return ret;
//}

//static void update_e_xy_and_e_xy_root(Fl_Window *nsw)
//{
//  NSPoint pt;
//  pt = [nsw mouseLocationOutsideOfEventStream];
//  float s = Fl::screen_driver()->scale(0);
//  Fl::e_x = int(pt.x / s);
//  Fl::e_y = int(([[nsw contentView] frame].size.height - pt.y)/s);
//  pt = [NSEvent mouseLocation];
//  Fl::e_x_root = int(pt.x/s);
//  Fl::e_y_root = int((main_screen_height - pt.y)/s);
//}

static void handleSDLMouseEvent(SDL_MouseButtonEvent& theEvent)
{
  static int px, py;

//  static int keysym[] = { 0, FL_Button+1, FL_Button+3, FL_Button+2 };
//  static int px, py;
  Fl::lock();
//  fl_lock_function();

  Fl_Window *window = Fl::first_window(); // TODO: SDL: the event is sent to the screen, so we need to find the window
//  Fl_Window *window = (Fl_Window*)[(FLWindow*)[theEvent window] getFl_Window];
//  if (!window || !window->shown() ) {
//    fl_unlock_function();
//    return;
//  }
  Fl_Window *first = Fl::first_window();
  if (first != window && !(first->modal() || first->non_modal())) Fl::first_window(window);
//  NSPoint pos = [theEvent locationInWindow];
//  float s = Fl::screen_driver()->scale(0);
//  pos.x /= s; pos.y /= s;
//  pos.y = window->h() - pos.y;
  Uint8 btn = theEvent.button;
//  NSUInteger mods = [theEvent modifierFlags];
  int sendEvent = 0;

  Uint32 etype = theEvent.type;
  if (etype == SDL_MOUSEBUTTONDOWN) {
    if (btn == SDL_BUTTON_LEFT) Fl::e_state |= FL_BUTTON1;
    else if (btn == SDL_BUTTON_MIDDLE) Fl::e_state |= FL_BUTTON2;
    else if (btn == SDL_BUTTON_RIGHT) Fl::e_state |= FL_BUTTON3;
  }
  else if (etype == SDL_MOUSEBUTTONUP) {
    if (btn == SDL_BUTTON_LEFT) {
      Fl::e_state &= ~FL_BUTTON1;
      Fl::e_keysym = FL_Button+1;
    } else if (btn == SDL_BUTTON_MIDDLE) {
      Fl::e_state &= ~FL_BUTTON2;
      Fl::e_keysym = FL_Button+2;
    } else if (btn == SDL_BUTTON_RIGHT) {
      Fl::e_state &= ~FL_BUTTON3;
      Fl::e_keysym = FL_Button+3;
    }
  }

  switch ( etype ) {
    case SDL_MOUSEBUTTONDOWN:
      sendEvent = FL_PUSH;
      Fl::e_is_click = 1;
      px = theEvent.x; py = theEvent.y;
      if (theEvent.clicks > 1)
        Fl::e_clicks++;
      else
        Fl::e_clicks = 0;
      // fall through
    case SDL_MOUSEBUTTONUP:
      if ( !window ) break;
      if ( !sendEvent ) {
        sendEvent = FL_RELEASE;
      }
      // fall through
//    case NSMouseMoved:
//      if ( !sendEvent ) {
//        sendEvent = FL_MOVE;
//      }
//      // fall through
//    case NSLeftMouseDragged:
//    case NSRightMouseDragged:
//    case NSOtherMouseDragged: {
//      if ( !sendEvent ) {
//        sendEvent = FL_MOVE; // Fl::handle will convert into FL_DRAG
//        if (fabs(pos.x-px)>5 || fabs(pos.y-py)>5)
//          Fl::e_is_click = 0;
//      }
//      mods_to_e_state( theEvent.state ); // TODO: SDL: event does not contain modifer data
//      update_e_xy_and_e_xy_root(window);
      Fl::e_x = Fl::e_x_root = theEvent.x;
      Fl::e_y = Fl::e_y_root = theEvent.y;
//      if (fl_mac_os_version < 100500) {
//        // before 10.5, mouse moved events aren't sent to borderless windows such as tooltips
//        Fl_Window *tooltip = Fl_Tooltip::current_window();
//        int inside = 0;
//        if (tooltip && tooltip->shown() ) { // check if a tooltip window is currently opened
//          // check if mouse is inside the tooltip
//          inside = (Fl::event_x_root() >= tooltip->x() && Fl::event_x_root() < tooltip->x() + tooltip->w() &&
//                    Fl::event_y_root() >= tooltip->y() && Fl::event_y_root() < tooltip->y() + tooltip->h() );
//        }
//        // if inside, send event to tooltip window instead of background window
//        if (inside)
//          window = tooltip;
//      }
      Fl::handle( sendEvent, window );
//    }
      break;
//    case NSMouseEntered :
//      Fl::handle(FL_ENTER, window);
//      break;
//    case NSMouseExited :
//      Fl::handle(FL_LEAVE, window);
//      break;
    default:
      break;
  }

  Fl::unlock();
//  fl_unlock_function();

  return;
}

static void handleSDLEvent(SDL_Event &event)
{
  switch (event.type) {
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
      handleSDLMouseEvent((SDL_MouseButtonEvent&)event);
      break;
  }
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
    handleSDLEvent(event);
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
