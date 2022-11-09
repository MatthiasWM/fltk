
#include "Fl_SDL_Graphics_Driver.H"


SDL_Renderer *fl_gc = NULL;

extern unsigned fl_cmap[256]; // defined in fl_color.cxx


Fl_SDL_Graphics_Driver::Fl_SDL_Graphics_Driver()
{
}

Fl_SDL_Graphics_Driver::~Fl_SDL_Graphics_Driver()
{
}

void Fl_SDL_Graphics_Driver::color(Fl_Color i)
{
  Fl_Graphics_Driver::color(i);
  uchar r, g, b, a;
  float fa = 1.0f;
  if (i & 0xFFFFFF00) {
    // translate rgb colors into color index
    r = i>>24;
    g = i>>16;
    b = i>> 8;
    a = 0;
  } else {
    // translate index into rgb:
    unsigned c = fl_cmap[i];
    c = c ^ 0x000000ff; // trick to restore the color's correct alpha value
    r = c>>24;
    g = c>>16;
    b = c>> 8;
    a = c & 0xff;
  }
  SDL_SetRenderDrawColor(fl_gc, r, g, b, a);
}

void Fl_SDL_Graphics_Driver::rectf(int x, int y, int w, int h)
{
  if (w<=0 || h<=0) return;
  SDL_Rect r = { x, y, w, h };
  SDL_RenderFillRect(fl_gc, &r);
}

void Fl_SDL_Graphics_Driver::xyline(int x, int y, int x1)
{
  SDL_RenderDrawLine(fl_gc, x, y, x1, y);
}

void Fl_SDL_Graphics_Driver::yxline(int x, int y, int y1)
{
  SDL_RenderDrawLine(fl_gc, x, y, x, y1);
  // SDL_RenderDrawLines(...)
}
