//
// Definition of SDL window driver.
//
// Copyright 2032 by Bill Spitzak and others.
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
#include "Fl_SDL_Window_Driver.H"
#include "../../Fl_Screen_Driver.H"
#include "Fl_SDL_Graphics_Driver.H"
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Overlay_Window.H>
#include <FL/Fl_Image_Surface.H>
#include <FL/Fl_Tooltip.H>
#include <FL/fl_draw.H>
#include <FL/Fl.H>
#include <FL/platform.H>
#include <math.h>


Window fl_window = NULL;


Fl_SDL_Window_Driver::Fl_SDL_Window_Driver(Fl_Window *w) : Fl_Window_Driver(w)
{
  wait_for_expose_value = 0;  // not needed under SDL
}

Fl_SDL_Window_Driver::~Fl_SDL_Window_Driver()
{

}

void Fl_SDL_Window_Driver::show() {
  Fl_X *top = NULL;
  if (parent()) top = Fl_X::flx(pWindow->top_window());
  if (!shown() && !parent()) {
    makeWindow();
  } else {
    if (!parent()) {
      // TODO: map and raise the window
    } else {
      // it's a subwindow, just mark it visible
      pWindow->set_visible();
    }
  }
}

void Fl_SDL_Window_Driver::hide() {
  Fl_X* ip = Fl_X::flx(pWindow);
//  if (ip && !parent()) pWindow->cursor(FL_CURSOR_DEFAULT);
  if ( hide_common() ) return;
//  q_release_context(this);
  if ( ip->xid == (fl_uintptr_t)fl_window )
    fl_window = 0;
  if (ip->region) Fl_Graphics_Driver::default_driver().XDestroyRegion(ip->region);
//  destroy((FLWindow*)ip->xid);
//  delete subRect();
  delete ip;
}

void Fl_SDL_Window_Driver::makeWindow()
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
//  if (w->border()) {
//    winstyle = NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask;
//    if (is_resizable())
//      winstyle |= NSResizableWindowMask;
//  } else {
//    winstyle = NSBorderlessWindowMask;
//  }
  if (show_iconic() && !w->parent()) { // prevent window from being out of work area when created iconized
    int sx, sy, sw, sh;
    Fl::screen_work_area (sx, sy, sw, sh, w->x(), w->y());
    if (w->x() < sx) x(sx);
    if (w->y() < sy) y(sy);
  }
  int xp = w->x();
  int yp = w->y();

  int xwm = xp, ywm = yp, bt, bx, by;

// The code below forces the window inside one of the screens(?!)
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
  if (force_position()) {
    if (!Fl::grab()) {
      xp = xwm; yp = ywm;
      x(xp);y(yp);
    }
    xp -= bx;
    yp -= by+bt;
  }

  Fl_X *x = new Fl_X;
  other_xid = 0; // room for doublebuffering image map.
  x->region = 0;
//  subRect(0);
//  gc = 0; // TODO: we need this for the graphics driver
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
  FLWindow *cw = (FLWindow*)this; // TODO: a little trick for now
//  FLWindow *cw = [[FLWindow alloc] initWithFl_W:w
//                                    contentRect:crect
//                                      styleMask:winstyle];
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
  flx(x);
  wait_for_expose_value = 0; // not needed
  if (!w->parent()) { // TODO: huh? Do we allow true subwindows?
    x->next = Fl_X::first;
    Fl_X::first = x;
  } else if (Fl_X::first) {
    x->next = Fl_X::first->next;
    Fl_X::first->next = x;
  } else {
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

//  // if (w->modal()) { Fl::modal_ = w; fl_fix_focus(); }
//  if (!w->parent()) [myview did_view_resolution_change]; // to set mapped_to_retina to its current state
//  [pool release];

  w->damage(FL_DAMAGE_ALL|FL_DAMAGE_EXPOSE);
}

void Fl_SDL_Window_Driver::make_current() {
  Fl_Window_Driver::make_current();
  Fl_Window *win = pWindow;
  Fl_SDL_Graphics_Driver &gc = (Fl_SDL_Graphics_Driver&)Fl_Graphics_Driver::default_driver();
  gc.sdl_update_screen = true;
  SDL_Rect r = { win->x(), win->y(), win->w(), win->h() };
  SDL_RenderSetViewport(gc.sdl_renderer, &r);
}

#if 0


Fl_Cocoa_Window_Driver::Fl_Cocoa_Window_Driver(Fl_Window *win)
: Fl_Window_Driver(win)
{
  cursor = nil;
  window_flags_ = 0;
  icon_image = NULL;
  screen_num_ = 0;
  shape_data_ = NULL;
}


void Fl_Cocoa_Window_Driver::take_focus()
{
  set_key_window();
}


void Fl_Cocoa_Window_Driver::flush_overlay()
{
  Fl_Overlay_Window *oWindow = pWindow->as_overlay_window();
  int erase_overlay = (pWindow->damage()&FL_DAMAGE_OVERLAY) | (overlay() == oWindow);
  pWindow->clear_damage((uchar)(pWindow->damage()&~FL_DAMAGE_OVERLAY));

  if (!oWindow->shown()) return;
  pWindow->make_current(); // make sure fl_gc is non-zero
  if (!other_xid) {
    other_xid = new Fl_Image_Surface(oWindow->w(), oWindow->h(), 1);
    oWindow->clear_damage(FL_DAMAGE_ALL);
  }
  if (oWindow->damage() & ~FL_DAMAGE_EXPOSE) {
    Fl_X *myi = Fl_X::flx(pWindow);
    fl_clip_region(myi->region); myi->region = 0;
    Fl_Surface_Device::push_current(other_xid);
    draw();
    Fl_Surface_Device::pop_current();
  }
  if (erase_overlay) fl_clip_region(0);
  fl_copy_offscreen(0, 0, oWindow->w(), oWindow->h(), other_xid->offscreen(), 0, 0);
  if (overlay() == oWindow) oWindow->draw_overlay();
}


void Fl_Cocoa_Window_Driver::draw_begin()
{
  if (!Fl_Surface_Device::surface()->driver()->has_feature(Fl_Graphics_Driver::NATIVE)) return;
  CGContextRef my_gc = (CGContextRef)Fl_Surface_Device::surface()->driver()->gc();
  if (shape_data_) {
# if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_4
    if (shape_data_->mask && (&CGContextClipToMask != NULL)) {
      CGContextClipToMask(my_gc, CGRectMake(0,0,w(),h()), shape_data_->mask); // requires Mac OS 10.4
    }
    CGContextSaveGState(my_gc);
# endif
  }
}


void Fl_Cocoa_Window_Driver::draw_end()
{
  // on OS X, windows have no frame. Before OS X 10.7, to resize a window, we drag the lower right
  // corner. This code draws a little ribbed triangle for dragging.
  if (fl_mac_os_version < 100700 && !parent() && pWindow->resizable() &&
      (!size_range_set() || minh() != maxh() || minw() != maxw())) {
    int dx = Fl::box_dw(pWindow->box())-Fl::box_dx(pWindow->box());
    int dy = Fl::box_dh(pWindow->box())-Fl::box_dy(pWindow->box());
    if (dx<=0) dx = 1;
    if (dy<=0) dy = 1;
    int x1 = w()-dx-1, x2 = x1, y1 = h()-dx-1, y2 = y1;
    Fl_Color c[4] = {
      pWindow->color(),
      fl_color_average(pWindow->color(), FL_WHITE, 0.7f),
      fl_color_average(pWindow->color(), FL_BLACK, 0.6f),
      fl_color_average(pWindow->color(), FL_BLACK, 0.8f),
    };
    int i;
    for (i=dx; i<12; i++) {
      fl_color(c[i&3]);
      fl_line(x1--, y1, x2, y2--);
    }
  }
# if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_4
  if (Fl_Surface_Device::surface()->driver()->has_feature(Fl_Graphics_Driver::NATIVE)) {
    CGContextRef my_gc = (CGContextRef)Fl_Surface_Device::surface()->driver()->gc();
    if (shape_data_) CGContextRestoreGState(my_gc);
  }
# endif
}



static void MyProviderReleaseData (void *info, const void *data, size_t size) {
  delete[] (uchar*)data;
}

// bitwise inversion of all 4-bit quantities
static const unsigned char swapped[16] = {0,8,4,12,2,10,6,14,1,9,5,13,3,11,7,15};

static inline uchar swap_byte(const uchar b) {
  // reverse the order of bits of byte b: 1->8 becomes 8->1
  return (swapped[b & 0xF] << 4) | swapped[b >> 4];
}


void Fl_Cocoa_Window_Driver::shape_bitmap_(Fl_Image* b) {
  shape_data_->shape_ = b;
  if (b) {
    // complement mask bits and perform bitwise inversion of all bytes and also reverse top and bottom
    int bytes_per_row = (b->w() + 7)/8;
    uchar *from = new uchar[bytes_per_row * b->h()];
    for (int i = 0; i < b->h(); i++) {
      uchar *p = (uchar*)(*b->data()) + bytes_per_row * i;
      uchar *last = p + bytes_per_row;
      uchar *q = from + (b->h() - 1 - i) * bytes_per_row;
      while (p < last) {
        *q++ = swap_byte(~*p++);
      }
    }
    CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, from, bytes_per_row * b->h(), MyProviderReleaseData);
    shape_data_->mask = CGImageMaskCreate(b->w(), b->h(), 1, 1, bytes_per_row, provider, NULL, false);
    CFRelease(provider);
  }
}


void Fl_Cocoa_Window_Driver::shape_alpha_(Fl_Image* img, int offset) {
  int i, d = img->d(), w = img->w(), h = img->h();
  shape_data_->shape_ = img;
  if (shape_data_->shape_) {
    // reverse top and bottom and convert to gray scale if img->d() == 3 and complement bits
    int bytes_per_row = w * d;
    uchar *from = new uchar[w * h];
    for ( i = 0; i < h; i++) {
      uchar *p = (uchar*)(*img->data()) + bytes_per_row * i + offset;
      uchar *last = p + bytes_per_row;
      uchar *q = from + (h - 1 - i) * w;
      while (p < last) {
        if (d == 3) {
          unsigned u = *p++;
          u += *p++;
          u += *p++;
          *q++ = ~(u/3);
        }
        else {
          *q++ = ~(*p);
          p += d;
        }
      }
    }
    CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, from, w * h, MyProviderReleaseData);
    shape_data_->mask = CGImageMaskCreate(w, h, 8, 8, w, provider, NULL, false);
    CFRelease(provider);
  }
}


void Fl_Cocoa_Window_Driver::shape(const Fl_Image* img) {
# if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_4
  if (shape_data_) {
    if (shape_data_->mask) { CGImageRelease(shape_data_->mask); }
  }
  else {
    shape_data_ = new shape_data_type;
  }
  memset(shape_data_, 0, sizeof(shape_data_type));
  int d = img->d();
  if (d && img->count() >= 2) {
    shape_pixmap_((Fl_Image*)img);
    shape_data_->shape_ = (Fl_Image*)img;
  }
  else if (d == 0) shape_bitmap_((Fl_Image*)img);
  else if (d == 2 || d == 4) shape_alpha_((Fl_Image*)img, d - 1);
  else if ((d == 1 || d == 3) && img->count() == 1) shape_alpha_((Fl_Image*)img, 0);
#endif
  pWindow->border(false);
}


int Fl_Cocoa_Window_Driver::scroll(int src_x, int src_y, int src_w, int src_h, int dest_x, int dest_y, void (*draw_area)(void*, int,int,int,int), void* data)
{
  if ( (src_x < 0) || (src_y < 0) )
    return 1;
  if ( (src_x+src_w > pWindow->w()) || (src_y+src_h > pWindow->h()) )
    return 1;
  CGImageRef img = CGImage_from_window_rect(src_x, src_y, src_w, src_h);
  if (!img)
    return 1;
  // the current surface is generally the display, but is an Fl_Image_Surface when scrolling an Fl_Overlay_Window
  Fl_Quartz_Graphics_Driver *qgd = (Fl_Quartz_Graphics_Driver*)Fl_Surface_Device::surface()->driver();
  float s = qgd->scale();
  qgd->draw_CGImage(img, dest_x, dest_y, (int)lround(s*src_w), (int)lround(s*src_h), 0, 0, src_w, src_h);
  CFRelease(img);
  return 0;
}

static const unsigned mapped_mask = 1;
static const unsigned changed_mask = 2;
static const unsigned view_resized_mask = 4;
static const unsigned through_resize_mask = 8;

bool Fl_Cocoa_Window_Driver::mapped_to_retina() {
  return window_flags_ & mapped_mask;
}

void Fl_Cocoa_Window_Driver::mapped_to_retina(bool b) {
  if (b) window_flags_ |= mapped_mask;
  else window_flags_ &= ~mapped_mask;
}

bool Fl_Cocoa_Window_Driver::changed_resolution() {
  return window_flags_ & changed_mask;
}

void Fl_Cocoa_Window_Driver::changed_resolution(bool b) {
  if (b) window_flags_ |= changed_mask;
  else window_flags_ &= ~changed_mask;
}

bool Fl_Cocoa_Window_Driver::view_resized() {
  return window_flags_ & view_resized_mask;
}

void Fl_Cocoa_Window_Driver::view_resized(bool b) {
  if (b) window_flags_ |= view_resized_mask;
  else window_flags_ &= ~view_resized_mask;
}

bool Fl_Cocoa_Window_Driver::through_resize() {
  return window_flags_ & through_resize_mask;
}

void Fl_Cocoa_Window_Driver::through_resize(bool b) {
  if (b) window_flags_ |= through_resize_mask;
  else window_flags_ &= ~through_resize_mask;
}


// clip the graphics context to rounded corners
void Fl_Cocoa_Window_Driver::clip_to_rounded_corners(CGContextRef gc, int w, int h) {
  const CGFloat radius = 7.5;
  CGContextMoveToPoint(gc, 0, 0);
  CGContextAddLineToPoint(gc, 0, h - radius);
  CGContextAddArcToPoint(gc, 0, h,  radius, h, radius);
  CGContextAddLineToPoint(gc, w - radius, h);
  CGContextAddArcToPoint(gc, w, h, w, h - radius, radius);
  CGContextAddLineToPoint(gc, w, 0);
  CGContextClip(gc);
}

const Fl_Image* Fl_Cocoa_Window_Driver::shape() {
  return shape_data_ ? shape_data_->shape_ : NULL;
}

/* Returns images of the capture of the window title-bar.
 On the Mac OS platform, left, bottom and right are returned NULL; top is returned with depth 4.
 */
void Fl_Cocoa_Window_Driver::capture_titlebar_and_borders(Fl_RGB_Image*& top, Fl_RGB_Image*& left, Fl_RGB_Image*& bottom, Fl_RGB_Image*& right)
{
  top = left = bottom = right = NULL;
  int htop, hleft, hright, hbottom;
  Fl_Cocoa_Window_Driver::decoration_sizes(&htop, &hleft,  &hright, &hbottom);
  if (htop == 0) return; // when window is fullscreen
  CGColorSpaceRef cspace = CGColorSpaceCreateDeviceRGB();
  float s = Fl::screen_driver()->scale(screen_num());
  int scaled_w = int(w() * s);
  const int factor = (mapped_to_retina() ? 2 : 1);
  int data_w = factor * scaled_w, data_h = factor * htop;
  uchar *rgba = new uchar[4 * data_w * data_h];
  CGContextRef auxgc = CGBitmapContextCreate(rgba, data_w, data_h, 8, 4 * data_w, cspace, kCGImageAlphaPremultipliedLast);
  CGColorSpaceRelease(cspace);
  CGContextClearRect(auxgc, CGRectMake(0,0,data_w,data_h));
  CGContextScaleCTM(auxgc, factor, factor);
  draw_titlebar_to_context(auxgc, scaled_w, htop);
  top = new Fl_RGB_Image(rgba, data_w, data_h, 4);
  top->alloc_array = 1;
  top->scale(w(),htop, s <1 ? 0 : 1, 1);
  CGContextRelease(auxgc);
}


FLWindow *fl_mac_xid(const Fl_Window *win) {
  return (FLWindow*)Fl_Window_Driver::xid(win);
}


Fl_Window *fl_mac_find(FLWindow *xid) {
  return Fl_Window_Driver::find((fl_uintptr_t)xid);
}


#endif
