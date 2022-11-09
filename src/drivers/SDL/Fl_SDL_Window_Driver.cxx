
#include "Fl_SDL_Window_Driver.H"

#include <FL/platform.H>
#include <FL/Fl_Tooltip.H>

FLWindow *fl_window;


FLWindow::FLWindow()
{
  // TODO: SDL: nothing here yet
}


Fl_SDL_Window_Driver::Fl_SDL_Window_Driver(Fl_Window *w)
: Fl_Window_Driver(w)
{
}

Fl_SDL_Window_Driver::~Fl_SDL_Window_Driver()
{
}

void Fl_SDL_Window_Driver::show()
{
  Fl_X *top = NULL;
  if (parent()) top = Fl_X::i(pWindow->top_window());
  if ( !shown() && (!parent()) ) { // TODO: SDL: || (top && ![(FLWindow*)top->xid isMiniaturized]))) {
    makeWindow();
  } else {
    if ( !parent() ) {
//      Fl_X *i = Fl_X::i(pWindow);
//      if ([(FLWindow*)i->xid isMiniaturized]) {
//        i->w->redraw();
//        [(FLWindow*)i->xid deminiaturize:nil];
//      }
//      if (!fl_capture) {
//        [(FLWindow*)i->xid makeKeyAndOrderFront:nil];
//      }
    }
    else pWindow->set_visible();
  }
}


Fl_X *Fl_SDL_Window_Driver::makeWindow()
{
//  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  Fl_Group::current(0);
  fl_open_display();
//  NSInteger winlevel = NSNormalWindowLevel;
//  NSUInteger winstyle;
//  Fl_Sys_Menu_Bar::create_window_menu(); // effective once at most
  Fl_Window* w = pWindow;
  if (w->parent()) {
    w->border(0);
    show_iconic(0);
  }
  if (w->border()) {
//    winstyle = NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask;
//    if (is_resizable())
//      winstyle |= NSResizableWindowMask;
  } else {
//    winstyle = NSBorderlessWindowMask;
  }
  if (show_iconic() && !w->parent()) { // prevent window from being out of work area when created iconized
    int sx, sy, sw, sh;
    Fl::screen_work_area (sx, sy, sw, sh, w->x(), w->y());
    if (w->x() < sx) x(sx);
    if (w->y() < sy) y(sy);
  }
  int xp = w->x();
  int yp = w->y();

  int xwm = xp, ywm = yp, bt, bx, by;

//  if (!fake_X_wm(w, xwm, ywm, bt, bx, by)) {
//    // menu windows and tooltips
//    if (w->modal()||w->tooltip_window()) {
//      winlevel = modal_window_level();
//    }
//  }
//  if (w->modal()) {
//    winstyle &= ~NSMiniaturizableWindowMask;
//    winlevel = modal_window_level();
//  }
//  else if (w->non_modal()) {
//    winlevel = non_modal_window_level();
//  }
//
//  if (force_position()) {
//    if (!Fl::grab()) {
//      xp = xwm; yp = ywm;
//      x(xp);y(yp);
//    }
//    xp -= bx;
//    yp -= by+bt;
//  }

  Fl_X *x = new Fl_X;
  other_xid = 0; // room for doublebuffering image map. On OS X this is only used by overlay windows
  x->region = 0;
//  subRect(0);
//  gc = 0;
//  mapped_to_retina(false);
//  changed_resolution(false);
//
//  NSRect crect;
//  if (w->fullscreen_active()) {
//    int top, bottom, left, right;
//    int sx, sy, sw, sh, X, Y, W, H;
//
//    top = fullscreen_screen_top();
//    bottom = fullscreen_screen_bottom();
//    left = fullscreen_screen_left();
//    right = fullscreen_screen_right();
//
//    if ((top < 0) || (bottom < 0) || (left < 0) || (right < 0)) {
//      top = Fl::screen_num(w->x(), w->y(), w->w(), w->h());
//      bottom = top;
//      left = top;
//      right = top;
//    }
//
//    Fl::screen_xywh(sx, sy, sw, sh, top);
//    Y = sy;
//    Fl::screen_xywh(sx, sy, sw, sh, bottom);
//    H = sy + sh - Y;
//    Fl::screen_xywh(sx, sy, sw, sh, left);
//    X = sx;
//    Fl::screen_xywh(sx, sy, sw, sh, right);
//    W = sx + sw - X;
//
//    w->resize(X, Y, W, H);
//
//    winstyle = NSBorderlessWindowMask;
//    winlevel = NSStatusWindowLevel;
//  }
//  float s = Fl::screen_driver()->scale(0);
//  crect.origin.x = round(s * w->x()); // correct origin set later for subwindows
//  crect.origin.y = main_screen_height - round(s * (w->y() + w->h()));
//  crect.size.width = int(s * w->w());
//  crect.size.height = int(s * w->h());
//  FLWindow *cw = [[FLWindow alloc] initWithFl_W:w
//                                    contentRect:crect
//                                      styleMask:winstyle];
  FLWindow *cw = new FLWindow;
//  [cw setFrameOrigin:crect.origin];
//#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_12
//  if (fl_mac_os_version >= 101200) {
//    if (!w->parent() && (winstyle & NSTitledWindowMask) && (winstyle & NSResizableWindowMask)
//        && !w->modal() && !w->non_modal() && Fl_MacOS_Sys_Menu_Bar_Driver::window_menu_style() > Fl_Sys_Menu_Bar::tabbing_mode_none) {
//      if (Fl_MacOS_Sys_Menu_Bar_Driver::window_menu_style() == Fl_Sys_Menu_Bar::tabbing_mode_preferred)
//        [cw setTabbingMode:NSWindowTabbingModePreferred];
//      else [cw setTabbingMode:NSWindowTabbingModeAutomatic];
//    } else {
//      [cw setTabbingMode:NSWindowTabbingModeDisallowed];
//    }
//  }
//#endif
//  if (!w->parent()) {
//    [cw setHasShadow:YES];
//    [cw setAcceptsMouseMovedEvents:YES];
//  }
//  if (w->shape()) {
//    [cw setOpaque:NO]; // shaped windows must be non opaque
//    [cw setBackgroundColor:[NSColor clearColor]]; // and with transparent background color
//  }
  x->xid = (fl_uintptr_t)cw;
  x->w = w;
  i(x);
//  wait_for_expose_value = 1;
  if (!w->parent()) {
    x->next = Fl_X::first;
    Fl_X::first = x;
  } else if (Fl_X::first) {
    x->next = Fl_X::first->next;
    Fl_X::first->next = x;
  }
  else {
    x->next = NULL;
    Fl_X::first = x;
  }
//  FLView *myview = [[FLView alloc] initWithFrame:crect];
//  [cw setContentView:myview];
//  [myview release];
//  [cw setLevel:winlevel];
//
//  q_set_window_title(cw, w->label(), w->iconlabel());
//  NSImage *icon = icon_image; // is a window or default icon present?
//  if (!icon) icon = ((Fl_Cocoa_Screen_Driver*)Fl::screen_driver())->default_icon;
//  if (icon && (winstyle & NSTitledWindowMask) && w->label() && strlen(w->label())>0) {
//    [cw setRepresentedFilename:[NSString stringWithFormat:@"/%@", [cw title]]];
//    NSButton *icon_button = [cw standardWindowButton:NSWindowDocumentIconButton];
//    if (icon_button) {
//      [icon setSize:[icon_button frame].size];
//      [icon_button setImage:icon];
//    }
//  }
//  if (!force_position()) {
//    if (w->modal()) {
//      [cw center];
//    } else if (w->non_modal()) {
//      [cw center];
//    } else if (!w->fullscreen_active()) {
//      static NSPoint delta = NSZeroPoint;
//      delta = [cw cascadeTopLeftFromPoint:delta];
//    }
//    crect = [cw frame]; // synchronize FLTK's and the system's window coordinates
//    this->x(round(crect.origin.x/s));
//    this->y( round((main_screen_height - crect.origin.y)/s) - w->h() );
//  }
//  if(w->menu_window()) { // make menu windows slightly transparent
//    [cw setAlphaValue:0.97];
//  }
//  // Install DnD handlers
//  [myview registerForDraggedTypes:[NSArray arrayWithObjects:UTF8_pasteboard_type,  NSFilenamesPboardType, nil]];

  if (size_range_set()) size_range();

  if ( w->border() || (!w->modal() && !w->tooltip_window()) ) {
    Fl_Tooltip::enter(0);
  }

  if (w->modal()) Fl::modal_ = w;

  w->set_visible();
  if ( w->border() || (!w->modal() && !w->tooltip_window()) ) Fl::handle(FL_FOCUS, w);
//  [cw setDelegate:[FLWindowDelegate singleInstance]];
//  if (show_iconic()) {
//    show_iconic(0);
//    w->handle(FL_SHOW); // create subwindows if any
//    if (fl_mac_os_version < 101300) { // TODO: threshold may be smaller
//      // draw the window and its subwindows before its icon is computed
//      [cw recursivelySendToSubwindows:@selector(display) applyToSelf:YES];
//    }
//    [cw miniaturize:nil];
//  } else if (w->parent()) { // a subwindow
//    [cw setIgnoresMouseEvents:YES]; // needs OS X 10.2
//    // next 2 statements so a subwindow doesn't leak out of its parent window
//    [cw setOpaque:NO];
//    [cw setBackgroundColor:[NSColor clearColor]]; // transparent background color
//    starting_moved_window = w;
//    [cw setSubwindowFrame];
//    starting_moved_window = NULL;
//    // needed if top window was first displayed miniaturized
//    FLWindow *pxid = fl_xid(w->top_window());
//    [pxid makeFirstResponder:[pxid contentView]];
//  } else { // a top-level window
//    [cw makeKeyAndOrderFront:nil];
//  }
//  if (fl_sys_menu_bar && Fl_MacOS_Sys_Menu_Bar_Driver::window_menu_style() && !w->parent() && w->border() &&
//      !w->modal() && !w->non_modal()) {
//    Fl_MacOS_Sys_Menu_Bar_Driver::driver()->new_window(w);
//  }
  int old_event = Fl::e_number;
  w->handle(Fl::e_number = FL_SHOW);
  Fl::e_number = old_event;

  w->damage(FL_DAMAGE_ALL); // TODO: SDL: here?
//  // if (w->modal()) { Fl::modal_ = w; fl_fix_focus(); }
//  if (!w->parent()) [myview did_view_resolution_change]; // to set mapped_to_retina to its current state
//  [pool release];
  return x;
}

void Fl_SDL_Window_Driver::make_current()
{
//  q_release_context();
  Fl_X *i = Fl_X::i(pWindow);
  //NSLog(@"region-count=%d damage=%u",i->region?i->region->count:0, pWindow->damage());
  fl_window = (FLWindow*)i->xid;
//  ((Fl_SDL_Graphics_Driver&)Fl_Graphics_Driver::default_driver()).high_resolution( mapped_to_retina() );
//
//  if (pWindow->as_overlay_window() && other_xid && changed_resolution()) {
//    destroy_double_buffer();
//    changed_resolution(false);
//  }
//#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_14
//  FLView *view = (FLView*)[fl_window contentView];
//  if (views_use_CA && !through_drawRect) { // detect direct calls from the app
//    [view setNeedsDisplay:YES];
//  }
//  if (views_use_CA && view->aux_bitmap) {
//    gc = view->aux_bitmap;
//  } else
//#endif
//  {
//    NSGraphicsContext *nsgc =   through_drawRect ? [NSGraphicsContext currentContext] : [NSGraphicsContext graphicsContextWithWindow:fl_window];
//    static SEL gc_sel = fl_mac_os_version >= 101000 ? @selector(CGContext) : @selector(graphicsPort);
//    gc = (CGContextRef)[nsgc performSelector:gc_sel];
//  }
//  Fl_Graphics_Driver::default_driver().gc(gc);
//#if defined(FLTK_HAVE_CAIROEXT)
//  CGContextSaveGState(gc); // one extra level
//#endif
//  CGContextSaveGState(gc); // native context
//  // antialiasing must be deactivated because it applies to rectangles too
//  // and escapes even clipping!!!
//  // it gets activated when needed (e.g., draw text)
//  CGContextSetShouldAntialias(gc, false);
//  CGFloat hgt = [[fl_window contentView] frame].size.height;
//  float s = Fl::screen_driver()->scale(0);
//  CGContextTranslateCTM(gc, 0.5f*s, hgt-0.5f*s);
//  CGContextScaleCTM(gc, 1.0f, -1.0f); // now 0,0 is top-left point of the window
//  CGContextScaleCTM(gc, s, s); // apply current scaling factor
//  // for subwindows, limit drawing to inside of parent window
//  // half pixel offset is necessary for clipping as done by fl_cgrectmake_cocoa()
//  if (subRect()) {
//    CGContextClipToRect(gc, CGRectOffset(*(subRect()), -0.5, -0.5));
//  }
//  // this is the context with origin at top left of (sub)window
//  CGContextSaveGState(gc);
//  fl_clip_region( 0 );
//#ifdef FLTK_HAVE_CAIROEXT
//  // update the cairo_t context
//  if (Fl::cairo_autolink_context()) Fl::cairo_make_current(pWindow);
//#endif
}

