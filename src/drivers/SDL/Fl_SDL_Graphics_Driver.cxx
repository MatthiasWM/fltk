//
// Rectangle drawing routines for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2018 by Bill Spitzak and others.
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
#include "Fl_SDL_Graphics_Driver.H"

#include <SDL_ttf.h>


static const char *font_file = "/usr/share/fonts/truetype/freefont/FreeSans.ttf";
// "/Library/Fonts/Arial Unicode.ttf"

Fl_SDL_Graphics_Driver::Fl_SDL_Graphics_Driver()
: sdl_surface(NULL),
  sdl_screen(NULL),
  sdl_renderer(NULL),
  sdl_texture(NULL),
  sdl_update_screen(true)
{
}

void Fl_SDL_Graphics_Driver::color(Fl_Color c) {
  Fl_Graphics_Driver::color(c);
  uchar r, g, b, a;
  Fl::get_color(c, r, g, b, a);
//  sdl_color = SDL_MapRGB(sdl_surface->format, r, g, b);
  SDL_SetRenderDrawColor(sdl_renderer, r, g, b, a);
}

void Fl_SDL_Graphics_Driver::rect(int x, int y, int w, int h) {
  if (w<=0 || h<=0) return;
  SDL_Rect rect = { x, y, w, h };
  SDL_RenderDrawRect(sdl_renderer, &rect);
}

void Fl_SDL_Graphics_Driver::rectf(int x, int y, int w, int h) {
  if (w<=0 || h<=0) return;
  SDL_Rect rect = { x, y, w, h };
  SDL_RenderFillRect(sdl_renderer, &rect);
//  SDL_FillRect(sdl_surface, &rect, sdl_color);
}

void Fl_SDL_Graphics_Driver::line(int x, int y, int x1, int y1) {
  SDL_RenderDrawLine(sdl_renderer, x, y, x1, y1);
}

void Fl_SDL_Graphics_Driver::line(int x, int y, int x1, int y1, int x2, int y2) {
  SDL_Point points[] = { { x, y }, { x1, y1 }, { x2, y2 } };
  SDL_RenderDrawLines(sdl_renderer, points, 3);
}

void Fl_SDL_Graphics_Driver::xyline(int x, int y, int x1) {
  SDL_RenderDrawLine(sdl_renderer, x, y, x1, y);
}

void Fl_SDL_Graphics_Driver::xyline(int x, int y, int x1, int y2) {
  SDL_Point points[] = { { x, y }, { x1, y }, { x1, y2 } };
  SDL_RenderDrawLines(sdl_renderer, points, 3);
}

void Fl_SDL_Graphics_Driver::xyline(int x, int y, int x1, int y2, int x3) {
  SDL_Point points[] = { { x, y }, { x1, y }, { x1, y2 }, { x3, y2 } };
  SDL_RenderDrawLines(sdl_renderer, points, 4);
}

void Fl_SDL_Graphics_Driver::yxline(int x, int y, int y1) {
  SDL_RenderDrawLine(sdl_renderer, x, y, x, y1);
}

void Fl_SDL_Graphics_Driver::yxline(int x, int y, int y1, int x2) {
  SDL_Point points[] = { { x, y }, { x, y1 }, { x2, y1 } };
  SDL_RenderDrawLines(sdl_renderer, points, 3);
}

void Fl_SDL_Graphics_Driver::yxline(int x, int y, int y1, int x2, int y3) {
  SDL_Point points[] = { { x, y }, { x, y1 }, { x2, y1 }, { x2, y3 } };
  SDL_RenderDrawLines(sdl_renderer, points, 4);
}

void Fl_SDL_Graphics_Driver::loop(int x0, int y0, int x1, int y1, int x2, int y2) {
  SDL_Point points[] = { { x0, y0 }, { x1, y1 }, { x2, y2 }, { x0, y0 } };
  SDL_RenderDrawLines(sdl_renderer, points, 4);
}

void Fl_SDL_Graphics_Driver::loop(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3) {
  SDL_Point points[] = { { x0, y0 }, { x1, y1 }, { x2, y2 }, { x3, y3 }, { x0, y0 } };
  SDL_RenderDrawLines(sdl_renderer, points, 5);
}

void Fl_SDL_Graphics_Driver::draw(const char *str, int n, int x, int y) {
  if (n==0) return;
  char *buf = (char*)str;
  if (str[n]!=0) {
    buf = (char*)::malloc(n+1);
    memcpy(buf, str, n);
    buf[n] = 0;
  }
  TTF_Font *font = TTF_OpenFont(font_file, size_); // TODO: on macOS
  uchar r, g, b;
  Fl::get_color(color_, r, g, b);
  SDL_Color color = { r, g, b };
  
  SDL_Surface *surface = TTF_RenderUTF8_Blended(font, buf, color); // TODO: n
  SDL_Texture *texture = SDL_CreateTextureFromSurface(sdl_renderer, surface);

  int texW = 0;
  int texH = 0;
  SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);

  int f_hgt = height();
  int f_ascent = f_hgt - descent();
  int f_full_ascent = (texH-f_hgt)*3/4; // random values to make text look centered vertically
  f_full_ascent += f_ascent;

  SDL_Rect dstrect = { x, y-f_full_ascent, texW, texH };
  SDL_RenderCopy(sdl_renderer, texture, NULL, &dstrect);

  SDL_DestroyTexture(texture);
  SDL_FreeSurface(surface);

  TTF_CloseFont(font);
  if (buf!=str) ::free(buf);
}

double Fl_SDL_Graphics_Driver::width(const char *str, int n) {
  if (n==0) return 0;
  char *buf = (char*)str;
  if (str[n]!=0) {
    buf = (char*)::malloc(n+1);
    memcpy(buf, str, n);
    buf[n] = 0;
  }
  TTF_Font *font = TTF_OpenFont(font_file, size_); // TODO: on macOS
  int w = 0, h = 0;
  TTF_SizeUTF8(font, buf, &w, &h);
  TTF_CloseFont(font);
  if (buf!=str) ::free(buf);
  return w;
}

int Fl_SDL_Graphics_Driver::height() {
  TTF_Font *font = TTF_OpenFont(font_file, size_); // TODO: on macOS
  int ret = TTF_FontHeight(font);
  TTF_CloseFont(font);
  return ret * 0.83f; // experimental values
}

int Fl_SDL_Graphics_Driver::descent() {
  TTF_Font *font = TTF_OpenFont(font_file, size_); // TODO: on macOS
  int ret = -TTF_FontDescent(font);
  TTF_CloseFont(font);
  return (ret + 2) * 0.83f; // experimental values
}



#if 0



#include "../Darwin/Fl_Darwin_System_Driver.H"
#include "../Cocoa/Fl_Cocoa_Screen_Driver.H"
#include <FL/platform.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Image_Surface.H>

#if HAS_ATSU && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5
Fl_Quartz_Graphics_Driver::pter_to_draw_member Fl_Quartz_Graphics_Driver::CoreText_or_ATSU_draw;
Fl_Quartz_Graphics_Driver::pter_to_width_member Fl_Quartz_Graphics_Driver::CoreText_or_ATSU_width;

int Fl_Quartz_Graphics_Driver::CoreText_or_ATSU = 0;

void Fl_Quartz_Graphics_Driver::init_CoreText_or_ATSU()
{
  if (Fl_Darwin_System_Driver::calc_mac_os_version() < 100500) {
    // before Mac OS 10.5, only ATSU is available
    CoreText_or_ATSU = use_ATSU;
    CoreText_or_ATSU_draw = &Fl_Quartz_Graphics_Driver::draw_ATSU;
    CoreText_or_ATSU_width = &Fl_Quartz_Graphics_Driver::width_ATSU;
  } else {
    CoreText_or_ATSU = use_CoreText;
    CoreText_or_ATSU_draw = &Fl_Quartz_Graphics_Driver::draw_CoreText;
    CoreText_or_ATSU_width = &Fl_Quartz_Graphics_Driver::width_CoreText;
  }
}
#endif


void Fl_Quartz_Graphics_Driver::antialias(int state) {
}

int Fl_Quartz_Graphics_Driver::antialias() {
  return 1;
}

Fl_Quartz_Graphics_Driver::Fl_Quartz_Graphics_Driver() : Fl_Graphics_Driver(), gc_(NULL) {
  quartz_line_width_ = 1.f;
  quartz_line_cap_ = kCGLineCapButt;
  quartz_line_join_ = kCGLineJoinMiter;
  quartz_line_pattern = 0;
  quartz_line_pattern_size = 0;
  high_resolution_ = false;
#if HAS_ATSU && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5
  if (!CoreText_or_ATSU) init_CoreText_or_ATSU();
#endif
}

char Fl_Quartz_Graphics_Driver::can_do_alpha_blending() {
  return 1;
}

static void bmProviderRelease (void *src, const void *data, size_t size) {
  CFIndex count = CFGetRetainCount(src);
  CFRelease(src);
  if(count == 1) free((void*)data);
}

/* Reference to the current CGContext
 For back-compatibility only. The preferred procedure to get this reference is
 Fl_Surface_Device::surface()->driver()->gc().
 */
CGContextRef fl_gc = 0;

void Fl_Quartz_Graphics_Driver::global_gc()
{
  fl_gc = (CGContextRef)gc();
}


CGContextRef fl_mac_gc() { return fl_gc; }


void Fl_Quartz_Graphics_Driver::copy_offscreen(int x, int y, int w, int h, Fl_Offscreen osrc, int srcx, int srcy) {
  // draw portion srcx,srcy,w,h of osrc to position x,y (top-left) of the graphics driver's surface
  CGContextRef src = (CGContextRef)osrc;
  void *data = CGBitmapContextGetData(src);
  int sw = (int)CGBitmapContextGetWidth(src);
  int sh = (int)CGBitmapContextGetHeight(src);
  CGImageRef img;
#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_4
  if (fl_mac_os_version >= 100400) img = CGBitmapContextCreateImage(src);  // requires 10.4
  else
#endif
  {
    CGImageAlphaInfo alpha = CGBitmapContextGetAlphaInfo(src);
    CGColorSpaceRef lut = CGColorSpaceCreateDeviceRGB();
    // when output goes to a Quartz printercontext, release of the bitmap must be
    // delayed after the end of the printed page
    CFRetain(src);
    CGDataProviderRef src_bytes = CGDataProviderCreateWithData( src, data, sw*sh*4, bmProviderRelease);
    img = CGImageCreate( sw, sh, 8, 4*8, 4*sw, lut, alpha,
                        src_bytes, 0L, false, kCGRenderingIntentDefault);
    CGDataProviderRelease(src_bytes);
    CGColorSpaceRelease(lut);
  }
  CGAffineTransform at = CGContextGetCTM(src);
  float s = at.a;
  draw_CGImage(img, x, y, w, h, srcx, srcy, sw/s, sh/s);
  CGImageRelease(img);
}

// so a CGRect matches exactly what is denoted x,y,w,h for clipping purposes
CGRect Fl_Quartz_Graphics_Driver::fl_cgrectmake_cocoa(int x, int y, int w, int h) {
  return CGRectMake(x - 0.5, y - 0.5, w, h);
}

void Fl_Quartz_Graphics_Driver::add_rectangle_to_region(Fl_Region r_, int X, int Y, int W, int H) {
  struct flCocoaRegion *r = (struct flCocoaRegion*)r_;
  CGRect arg = Fl_Quartz_Graphics_Driver::fl_cgrectmake_cocoa(X, Y, W, H);
  int j; // don't add a rectangle totally inside the Fl_Region
  for(j = 0; j < r->count; j++) {
    if(CGRectContainsRect(r->rects[j], arg)) break;
  }
  if( j >= r->count) {
    r->rects = (CGRect*)realloc(r->rects, (++(r->count)) * sizeof(CGRect));
    r->rects[r->count - 1] = arg;
  }
}

Fl_Region Fl_Quartz_Graphics_Driver::XRectangleRegion(int x, int y, int w, int h) {
  struct flCocoaRegion* R = (struct flCocoaRegion*)malloc(sizeof(struct flCocoaRegion));
  R->count = 1;
  R->rects = (CGRect *)malloc(sizeof(CGRect));
  *(R->rects) = Fl_Quartz_Graphics_Driver::fl_cgrectmake_cocoa(x, y, w, h);
  return R;
}

void Fl_Quartz_Graphics_Driver::XDestroyRegion(Fl_Region r_) {
  if (r_) {
    struct flCocoaRegion *r = (struct flCocoaRegion*)r_;
    free(r->rects);
    free(r);
  }
}

void Fl_Quartz_Graphics_Driver::cache_size(Fl_Image *img, int &width, int &height) {
  width *= 2 * scale();
  height *= 2 * scale();
}

float Fl_Quartz_Graphics_Driver::override_scale() {
  float s = scale();
  if (s != 1.f && Fl_Display_Device::display_device()->is_current()) {
    Fl::screen_driver()->scale(0, 1.f);
    CGContextScaleCTM(gc_, 1/s, 1/s);
  }
  return s;
}

void Fl_Quartz_Graphics_Driver::restore_scale(float s) {
  if (s != 1.f && Fl_Display_Device::display_device()->is_current()) {
    Fl::screen_driver()->scale(0, s);
    CGContextScaleCTM(gc_, s, s);
  }
}



#endif



