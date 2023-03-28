//
// Python bindings for the Fast Light Tool Kit (FLTK).
//
// Copyright 2023 by Bill Spitzak and others.
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

#include "Flpy.h"

#include <FL/Fl_Widget.H>
#include <FL/Fl_Box.H>

class Flpy_Widget;
class Flpy_Derived_Widget;

typedef struct {
  FlpyObject_HEAD
  Flpy_Widget *o;
} Flpy_Widget_Object;


class Flpy_Widget : public Fl_Box {
public: // 137 methods
  Flpy_Widget(int x, int y, int w, int h) : Fl_Box(x, y, w, h) { }
  void label(const char *str) { copy_label(str); }
  const char *label() { return label(); }
  void tooltip(const char *str) { copy_tooltip(str); }
  const char *tooltip() { return tooltip(); }
  static PyObject *flpy_callback(Flpy_Widget_Object *self, PyObject *args) {
    if (PyTuple_Size(args) == 0) {
      return self->callback_method;
    } else {
      self->callback_method = PyTuple_GetItem(args, 0);
      int v = PyCallable_Check(self->callback_method);
      Py_INCREF(self->callback_method);
      if (PyTuple_Size(args) == 1) {
        self->callback_data = Py_None;
      } else {
        self->callback_data = PyTuple_GetItem(args, 1);
        Py_XINCREF(self->callback_data);
      }
      Py_RETURN_NONE;
    }
    return NULL;
  }
  static int flpy_init(Flpy_Widget_Object *self, PyObject *args, PyObject*) {
    int x, y, w, h;
    if (!PyArg_ParseTuple(args, "iiii", &x, &y, &w, &h)) return -1;
    Fl_Widget *o = self->o = new Flpy_Widget(x, y, w, h);
    o->callback(flpy_on_callback, self);
    if (o->parent()) Py_INCREF(self);
    return 0;
  }
  static PyMethodDef flpy_methods[];
};

PyMethodDef Flpy_Widget::flpy_methods[] = {
  //  Fl_Widget(int x, int y, int w, int h, const char *label=0L);
  //  virtual ~Fl_Widget();
  Flpy_METHOD_v_TO_I(Widget, flags),
  Flpy_METHOD_I_TO_v_TYPE(Widget, set_flag, unsigned int),
  Flpy_METHOD_I_TO_v_TYPE(Widget, clear_flag, unsigned int),
  //  void draw_box() const;
  //  void draw_box(Fl_Boxtype t, Fl_Color c) const;
  //  void draw_box(Fl_Boxtype t, int x,int y,int w,int h, Fl_Color c) const;
  Flpy_METHOD_v_TO_v(Widget, draw_backdrop),
  //  void draw_focus() const;
  //  void draw_focus(Fl_Boxtype t, int X, int Y, int W, int H) const;
  //  void draw_focus(Fl_Boxtype t, int x, int y, int w, int h, Fl_Color bg) const;
  //  void draw_label() const;
  //  void draw_label(int, int, int, int) const;
  Flpy_METHOD_v_TO_v(Widget, draw), //  virtual void draw() = 0;
  //  virtual int handle(int event);
  Flpy_METHOD_v_TO_i(Widget, is_label_copied),
  //  Fl_Group* parent() const {return parent_;}
  //  void parent(Fl_Group* p) {parent_ = p;} // for hacks only, use Fl_Group::add()
  Flpy_METHOD_v_TO_I_OR_I_TO_v_TYPE(Widget, type, uchar),
  Flpy_METHOD_v_TO_i_OR_i_TO_v(Widget, x),
  Flpy_METHOD_v_TO_i_OR_i_TO_v(Widget, y),
  Flpy_METHOD_v_TO_i_OR_i_TO_v(Widget, w),
  Flpy_METHOD_v_TO_i_OR_i_TO_v(Widget, h),
  //  virtual void resize(int x, int y, int w, int h);
  //  int damage_resize(int,int,int,int);
  //  void position(int X,int Y) {resize(X,Y,w_,h_);}
  //  void size(int W,int H) {resize(x_,y_,W,H);}
  { "size", [](PyObject *self, PyObject *args)->PyObject* {
    int w, h;
    if (PyArg_ParseTuple(args, "ii:Fl_Widget.size()", &w, &h)) {
      ((Flpy_Widget_Object*)self)->o->size(w, h);
      Py_RETURN_NONE;
    }
    return NULL;
  }, METH_VARARGS },
  Flpy_METHOD_v_TO_I_OR_I_TO_v_TYPE(Widget, align, Fl_Align),
  Flpy_METHOD_v_TO_I_OR_I_TO_v_TYPE(Widget, box, Fl_Boxtype),
  Flpy_METHOD_v_TO_I_OR_I_TO_v_TYPE(Widget, color, Fl_Color), // TODO: see color() call below
  Flpy_METHOD_v_TO_I_OR_I_TO_v_TYPE(Widget, selection_color, Fl_Color),
  //  void color(Fl_Color bg, Fl_Color sel) {color_=bg; color2_=sel;}
  Flpy_METHOD_v_TO_z_OR_z_TO_v(Widget, label),
  //  void label(Fl_Labeltype a, const char* b) {label_.type = a; label_.value = b;} // TOD: another label() call
  Flpy_METHOD_v_TO_I_OR_I_TO_v_TYPE(Widget, labeltype, Fl_Labeltype),
  Flpy_METHOD_v_TO_I_OR_I_TO_v_TYPE(Widget, labelcolor, Fl_Color),
  Flpy_METHOD_v_TO_i_OR_i_TO_v(Widget, labelfont),
  Flpy_METHOD_v_TO_i_OR_i_TO_v(Widget, labelsize),
  //  Fl_Image* image() {return label_.image;}
  //  const Fl_Image* image() const {return label_.image;}
  //  void image(Fl_Image* img);
  //  void image(Fl_Image& img);
  //  void bind_image(Fl_Image* img);
  //  void bind_image(int f) { if (f) set_flag(IMAGE_BOUND); else clear_flag(IMAGE_BOUND); }
  //  int image_bound() const {return ((flags_ & IMAGE_BOUND) ? 1 : 0);}
  //  Fl_Image* deimage() {return label_.deimage;}
  //  const Fl_Image* deimage() const {return label_.deimage;}
  //  void deimage(Fl_Image* img);
  //  void deimage(Fl_Image& img);
  //  void bind_deimage(Fl_Image* img);
  //  int deimage_bound() const {return ((flags_ & DEIMAGE_BOUND) ? 1 : 0);}
  //  void bind_deimage(int f) { if (f) set_flag(DEIMAGE_BOUND); else clear_flag(DEIMAGE_BOUND); }
  Flpy_METHOD_v_TO_z_OR_z_TO_v(Widget, tooltip),
  //  Fl_Callback_p callback() const {return callback_;}
  { "callback", (PyCFunction)Flpy_Widget::flpy_callback, METH_VARARGS },
  //  void callback(Fl_Callback* cb, void* p) {callback_ = cb; user_data_ = p;}
  //  void callback(Fl_Callback* cb) {callback_ = cb;}
  //  void callback(Fl_Callback0* cb) {callback_ = (Fl_Callback*)cb;}
  //  void callback(Fl_Callback1* cb, long p = 0);
  //  void* user_data() const {return user_data_;}
  //  void user_data(void* v) {user_data_ = v;}
  //  long argument() const {return (long)(fl_intptr_t)user_data_;}
  //  void argument(long v) {user_data_ = (void*)(fl_intptr_t)v;}
  Flpy_METHOD_v_TO_I_OR_I_TO_v_TYPE(Widget, when, uchar),
  Flpy_METHOD_v_TO_I(Widget, visible),
  Flpy_METHOD_v_TO_i(Widget, visible_r),
  Flpy_METHOD_v_TO_v(Widget, show),//  virtual void show();
  Flpy_METHOD_v_TO_v(Widget, hide),//  virtual void hide();
  Flpy_METHOD_v_TO_v(Widget, set_visible),
  Flpy_METHOD_v_TO_v(Widget, clear_visible),
  Flpy_METHOD_v_TO_I(Widget, active),
  Flpy_METHOD_v_TO_i(Widget, active_r),
  Flpy_METHOD_v_TO_v(Widget, activate),
  Flpy_METHOD_v_TO_v(Widget, deactivate),
  Flpy_METHOD_v_TO_I(Widget, output),
  Flpy_METHOD_v_TO_v(Widget, set_output),
  Flpy_METHOD_v_TO_v(Widget, clear_output),
  Flpy_METHOD_v_TO_I(Widget, takesevents),
  Flpy_METHOD_v_TO_I(Widget, changed),
  Flpy_METHOD_v_TO_v(Widget, set_changed),
  Flpy_METHOD_v_TO_v(Widget, clear_changed),
  Flpy_METHOD_v_TO_v(Widget, clear_active),
  Flpy_METHOD_v_TO_v(Widget, set_active),
  Flpy_METHOD_v_TO_i(Widget, take_focus),
  Flpy_METHOD_v_TO_v(Widget, set_visible_focus),
  Flpy_METHOD_v_TO_v(Widget, clear_visible_focus),
  Flpy_METHOD_v_TO_i_OR_i_TO_v(Widget, visible_focus),
  //  void do_callback(Fl_Callback_Reason reason=FL_REASON_UNKNOWN) {do_callback(this, user_data_, reason);}
  //  void do_callback(Fl_Widget *widget, long arg, Fl_Callback_Reason reason=FL_REASON_UNKNOWN);
  //  void do_callback(Fl_Widget *widget, void *arg = 0, Fl_Callback_Reason reason=FL_REASON_UNKNOWN);
  Flpy_METHOD_v_TO_i(Widget, test_shortcut),
  //  void _set_fullscreen() {flags_ |= FULLSCREEN;}
  //  void _clear_fullscreen() {flags_ &= ~FULLSCREEN;}
  //  int contains(const Fl_Widget *w) const ;
  //  int inside(const Fl_Widget *wgt) const {return wgt ? wgt->contains(this) : 0;}
  Flpy_METHOD_v_TO_v(Widget, redraw),
  Flpy_METHOD_v_TO_v(Widget, redraw_label),
  //  void clear_damage(uchar c = 0) {damage_ = c;}
  //  uchar damage() const {return damage_;}
  //  void damage(uchar c);
  //  void damage(uchar c, int x, int y, int w, int h);
  //  void draw_label(int, int, int, int, Fl_Align) const;
  //  void measure_label(int& ww, int& hh) const {label_.measure(ww, hh);}
  Flpy_METHOD_v_TO_w(Widget, window),
  Flpy_METHOD_v_TO_w(Widget, top_window),
  //  Fl_Window* top_window_offset(int& xoff, int& yoff) const;
  Flpy_METHOD_v_TO_w(Widget, as_group),
  Flpy_METHOD_v_TO_w(Widget, as_window),
  Flpy_METHOD_v_TO_w(Widget, as_gl_window),
  Flpy_METHOD_v_TO_i(Widget, use_accents_menu),
  Flpy_METHOD_v_TO_I_OR_I_TO_v_TYPE(Widget, color2, Fl_Color),
  Flpy_METHOD_v_TO_i_OR_i_TO_v(Widget, shortcut_label),
  //  static void default_callback(Fl_Widget *widget, void *data);
  //  static unsigned int label_shortcut(const char *t);
  //  static int test_shortcut(const char*, const bool require_alt = false);
  { NULL }
};

class Flpy_Derived_Widget : public Flpy_Widget {
public:
  Flpy_Derived_Widget(int x, int y, int w, int h) : Flpy_Widget(x, y, w, h) { }
};

/*
 INACTIVE        = 1<<0,   ///< the widget can't receive focus, and is disabled but potentially visible
 INVISIBLE       = 1<<1,   ///< the widget is not drawn, but can receive a few special events
 OUTPUT          = 1<<2,   ///< for output only
 NOBORDER        = 1<<3,   ///< don't draw a decoration (Fl_Window)
 FORCE_POSITION  = 1<<4,   ///< don't let the window manager position the window (Fl_Window)
 NON_MODAL       = 1<<5,   ///< this is a hovering toolbar window (Fl_Window)
 SHORTCUT_LABEL  = 1<<6,   ///< the label contains a shortcut we need to draw
 CHANGED         = 1<<7,   ///< the widget value changed
 OVERRIDE        = 1<<8,   ///< position window on top (Fl_Window)
 VISIBLE_FOCUS   = 1<<9,   ///< accepts keyboard focus navigation if the widget can have the focus
 COPIED_LABEL    = 1<<10,  ///< the widget label is internally copied, its destruction is handled by the widget
 CLIP_CHILDREN   = 1<<11,  ///< all drawing within this widget will be clipped (Fl_Group)
 MENU_WINDOW     = 1<<12,  ///< a temporary popup window, dismissed by clicking outside (Fl_Window)
 TOOLTIP_WINDOW  = 1<<13,  ///< a temporary popup, transparent to events, and dismissed easily (Fl_Window)
 MODAL           = 1<<14,  ///< a window blocking input to all other winows (Fl_Window)
 NO_OVERLAY      = 1<<15,  ///< window not using a hardware overlay plane (Fl_Menu_Window)
 GROUP_RELATIVE  = 1<<16,  ///< Reserved, not implemented. DO NOT USE.
 COPIED_TOOLTIP  = 1<<17,  ///< the widget tooltip is internally copied, its destruction is handled by the widget
 FULLSCREEN      = 1<<18,  ///< a fullscreen window (Fl_Window)
 MAC_USE_ACCENTS_MENU = 1<<19, ///< On the Mac OS platform, pressing and holding a key on the keyboard opens an accented-character menu window (Fl_Input_, Fl_Text_Editor)
 NEEDS_KEYBOARD  = 1<<20,  ///< set this on touch screen devices if a widget needs a keyboard when it gets Focus. @see Fl_Screen_Driver::request_keyboard()
 IMAGE_BOUND     = 1<<21,  ///< binding the image to the widget will transfer ownership, so that the widget will delete the image when it is no longer needed
 DEIMAGE_BOUND   = 1<<22,  ///< bind the inactive image to the widget, so the widget deletes the image when it no longer needed
 USERFLAG3       = 1<<29,  ///< reserved for 3rd party extensions
 USERFLAG2       = 1<<30,  ///< reserved for 3rd party extensions
 USERFLAG1       = 1<<31   ///< reserved for 3rd party extensions
 */

PyTypeObject flpy_widget_type = {
  .ob_base = { PyObject_HEAD_INIT(NULL) },
  .tp_name = "fltk.Fl_Widget",
  .tp_basicsize = sizeof(Flpy_Widget_Object),
  .tp_itemsize = 0,
  .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
  .tp_doc = PyDoc_STR("Fl_Widget"),
  .tp_methods = Flpy_Widget::flpy_methods,
  .tp_init = (initproc)Flpy_Widget::flpy_init,
  .tp_new = PyType_GenericNew,
};

void flpy_on_callback(Fl_Widget *w, void *v) {
  Flpy_Widget_Object *self = (Flpy_Widget_Object*)v;
  if (self->callback_method) {
    // if callback_obj is set, call the method in an object with self, widget, user_data
    // else call the function with widget, user_data
    // NOTE: if we allow an arbitrary number of parameters in Fl_Widget.callback(),
    // we can create a tuple here with an arbitray number of parameters, and so the callback can have that too.
    PyObject *tuple = PyTuple_Pack(1, self->callback_data);
    Py_INCREF(tuple);
    PyObject *result = PyObject_CallObject(self->callback_method, tuple); //self->callback_data);
    Py_DECREF(tuple);
  }
  // NOTE: we could also test if self.on_callback exists and call that. Would that be easier?
}

