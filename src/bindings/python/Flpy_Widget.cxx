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
#include <FL/Fl_Group.H>
#include <FL/Fl_Gl_Window.H>

class Flpy_Widget;
class Flpy_Derived_Widget;

typedef struct {
  FlpyObject_HEAD
  Flpy_Widget *o;
} Flpy_Widget_Object;

class Flpy_Widget : public Fl_Box {
public: // 137 methods
  Flpy_Widget(int x, int y, int w, int h) : Fl_Box(x, y, w, h) { }
//  void label(const char *str) { copy_label(str); }
//  const char *label() { return label(); }
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
  static PyObject *flpy_damage(PyObject *self, PyObject *args) {
    Flpy_ARG_v_TO_I(Widget, damage)
    Flpy_ARG_I_TO_v_TYPE(Widget, damage, uchar)
    Flpy_ARG_Iiiii_TO_v_TYPE(Widget, damage, uchar)
    Flpy_ERR_IMPL(Widget, damage, "arguments are none, flags, or flags and x, y, w, h")
  }
  static int flpy_init(Flpy_Widget_Object *self, PyObject *args, PyObject*) {
    int x, y, w, h;
    if (!PyArg_ParseTuple(args, "iiii", &x, &y, &w, &h)) return -1;
    Fl_Widget *o = self->o = new Flpy_Widget(x, y, w, h);
    o->callback(flpy_on_callback, self);
    if (o->parent()) Py_INCREF(self);
    return 0;
  }
  static PyObject *flpy_measure_label(Flpy_Widget_Object *self, PyObject *args) {
    int arg0 = 0, arg1 = 0;
    if (!PyArg_ParseTuple(args, "|i", &arg0)) return NULL;
    self->o->measure_label(arg0, arg1);
    return Py_BuildValue("(ii)", arg0, arg1);
  }
  static PyObject *flpy_top_window_offset(Flpy_Widget_Object *self, PyObject *args) {
    int arg0 = 0, arg1 = 0;
    Fl_Window *win = self->o->top_window_offset(arg0, arg1);
    PyObject *win_obj = win ? (PyObject*)win->user_data() : Py_None ;
    return Py_BuildValue("(Oii)", win_obj, arg0, arg1);
  }
  static PyMethodDef flpy_methods[];
};

PyMethodDef Flpy_Widget::flpy_methods[] = {

  //    Fl_Widget(int x, int y, int w, int h, const char *label=0L);
  //    virtual ~Fl_Widget();
  FlpyMETHOD_VARARGS_BEGIN(Widget, x)
    FlpyARG_i_TO_v(Widget, x)
    FlpyARG_v_TO_i(Widget, x)
  FlpyMETHOD_VARARGS_END(Widget, x, "takes no arguments or a single integer"),
  FlpyMETHOD_VARARGS_BEGIN(Widget, y)
    FlpyARG_i_TO_v(Widget, y)
    FlpyARG_v_TO_i(Widget, y)
  FlpyMETHOD_VARARGS_END(Widget, y, "takes no arguments or a single integer"),
  FlpyMETHOD_VARARGS_BEGIN(Widget, w)
    FlpyARG_i_TO_v(Widget, w)
    FlpyARG_v_TO_i(Widget, w)
  FlpyMETHOD_VARARGS_END(Widget, w, "takes no arguments or a single integer"),
  FlpyMETHOD_VARARGS_BEGIN(Widget, h)
    FlpyARG_i_TO_v(Widget, h)
    FlpyARG_v_TO_i(Widget, h)
  FlpyMETHOD_VARARGS_END(Widget, h, "takes no arguments or a single integer"),
  FlpyMETHOD_v_TO_I(Widget, flags),
  FlpyMETHOD_I_TO_v(Widget, set_flag),
  FlpyMETHOD_I_TO_v(Widget, clear_flag),
  FlpyMETHOD_VARARGS_BEGIN(Widget, draw_box)
    FlpyARG_v_TO_v(Widget, draw_box)
    FlpyARG_II_TO_v_TYPE(Widget, draw_box, Fl_Boxtype, Fl_Color)
    FlpyARG_IiiiiI_TO_v_TYPE(Widget, draw_box, Fl_Boxtype, Fl_Color)
  FlpyMETHOD_VARARGS_END(Widget, draw_box, "takes no arguments, or a boxtype and a color, or a boxtype, a box, and a color"),
  FlpyMETHOD_v_TO_v(Widget, draw_backdrop),
  FlpyMETHOD_VARARGS_BEGIN(Widget, draw_focus)
    FlpyARG_v_TO_v(Widget, draw_focus)
    FlpyARG_Iiiii_TO_v_TYPE(Widget, draw_focus, Fl_Boxtype)
    FlpyARG_IiiiiI_TO_v_TYPE(Widget, draw_focus, Fl_Boxtype, Fl_Color)
  FlpyMETHOD_VARARGS_END(Widget, draw_box, "takes no arguments, or a boxtype, a box, and an optional color"),
  FlpyMETHOD_VARARGS_BEGIN(Widget, draw_label)
    FlpyARG_v_TO_v(Widget, draw_label)
    FlpyARG_iiii_TO_v(Widget, draw_label)
  FlpyMETHOD_VARARGS_END(Widget, draw_box, "takes no arguments, or box dimensions"),
  FlpyMETHOD_VIRT_v_TO_v(Widget, draw),
  FlpyMETHOD_VIRT_i_TO_i(Widget, handle),
//  Not used:  int is_label_copied() const {return ((flags_ & COPIED_LABEL) ? 1 : 0);}
  FlpyMETHOD_VARARGS_BEGIN(Widget, parent)
    FlpyARG_v_TO_w(Widget, parent)
    FlpyARG_g_TO_v(Widget, parent) // FIXME: this method may change ownership and must update refcounts
  FlpyMETHOD_VARARGS_END(Widget, draw_box, "takes no argument, or a group widget"),
  FlpyMETHOD_VARARGS_BEGIN(Widget, type)
    FlpyARG_v_TO_i(Widget, type)
    FlpyARG_I_TO_v_TYPE(Widget, type, uchar)
  FlpyMETHOD_VARARGS_END(Widget, draw_box, "takes no argument, or an unsigned integer"),
  FlpyMETHOD_VIRT_iiii_TO_v(Widget, resize),
  FlpyMETHOD_iiii_TO_i(Widget, damage_resize),
  FlpyMETHOD_ii_TO_v(Widget, position),
  FlpyMETHOD_ii_TO_v(Widget, size),
  FlpyMETHOD_VARARGS_BEGIN(Widget, align)
    FlpyARG_I_TO_v_TYPE(Widget, align, Fl_Align)
    FlpyARG_v_TO_i(Widget, align)
  FlpyMETHOD_VARARGS_END(Widget, align, "takes no arguments or a single alignment value"),
  FlpyMETHOD_VARARGS_BEGIN(Widget, box)
    FlpyARG_I_TO_v_TYPE(Widget, box, Fl_Boxtype)
    FlpyARG_v_TO_i(Widget, box)
  FlpyMETHOD_VARARGS_END(Widget, box, "takes no arguments or a boxtype"),
  FlpyMETHOD_VARARGS_BEGIN(Widget, color)
    FlpyARG_I_TO_v_TYPE(Widget, color, Fl_Color)
    FlpyARG_II_TO_v_TYPE(Widget, color, Fl_Color, Fl_Color)
    FlpyARG_v_TO_i(Widget, color)
  FlpyMETHOD_VARARGS_END(Widget, color, "takes no arguments, or one or two colors"),
  FlpyMETHOD_VARARGS_BEGIN(Widget, selection_color)
    FlpyARG_I_TO_v_TYPE(Widget, selection_color, Fl_Color)
    FlpyARG_v_TO_i(Widget, selection_color)
  FlpyMETHOD_VARARGS_END(Widget, selection_color, "takes no arguments or one color"),
  FlpyMETHOD_VARARGS_BEGIN(Widget, label)
    FlpyARG_z_TO_v(Widget, copy_label)
    FlpyARG_v_TO_z(Widget, label)
  FlpyMETHOD_VARARGS_END(Widget, label, "takes no arguments or a single text string"),
//    void label(Fl_Labeltype a, const char* b) {label_.type = a; label_.value = b;}
  FlpyMETHOD_VARARGS_BEGIN(Widget, labeltype)
    FlpyARG_I_TO_v_TYPE(Widget, labeltype, Fl_Labeltype)
    FlpyARG_v_TO_i(Widget, labeltype)
  FlpyMETHOD_VARARGS_END(Widget, labeltype, "takes no arguments or one labeltype"),
  FlpyMETHOD_VARARGS_BEGIN(Widget, labelcolor)
    FlpyARG_I_TO_v_TYPE(Widget, labelcolor, Fl_Color)
    FlpyARG_v_TO_i(Widget, labelcolor)
  FlpyMETHOD_VARARGS_END(Widget, labelcolor, "takes no arguments or one color"),
  FlpyMETHOD_VARARGS_BEGIN(Widget, labelfont)
    FlpyARG_I_TO_v_TYPE(Widget, labelfont, Fl_Font)
    FlpyARG_v_TO_i(Widget, labelfont)
  FlpyMETHOD_VARARGS_END(Widget, labelfont, "takes no arguments or a font index"),
  FlpyMETHOD_VARARGS_BEGIN(Widget, labelsize)
    FlpyARG_i_TO_v(Widget, labelsize)
    FlpyARG_v_TO_i(Widget, labelsize)
  FlpyMETHOD_VARARGS_END(Widget, labelsize, "takes no arguments or a text size"),
//    Fl_Image* image() {return label_.image;}
//    const Fl_Image* image() const {return label_.image;}
//    void image(Fl_Image* img);
//    void image(Fl_Image& img);
//    void bind_image(Fl_Image* img);
//    void bind_image(int f) { if (f) set_flag(IMAGE_BOUND); else clear_flag(IMAGE_BOUND); }
//    int image_bound() const {return ((flags_ & IMAGE_BOUND) ? 1 : 0);}
//    Fl_Image* deimage() {return label_.deimage;}
//    const Fl_Image* deimage() const {return label_.deimage;}
//    void deimage(Fl_Image* img);
//    void deimage(Fl_Image& img);
//    void bind_deimage(Fl_Image* img);
//    int deimage_bound() const {return ((flags_ & DEIMAGE_BOUND) ? 1 : 0);}
//    void bind_deimage(int f) { if (f) set_flag(DEIMAGE_BOUND); else clear_flag(DEIMAGE_BOUND); }
  FlpyMETHOD_VARARGS_BEGIN(Widget, tooltip)
    FlpyARG_z_TO_v(Widget, copy_tooltip)
    FlpyARG_v_TO_z(Widget, tooltip)
  FlpyMETHOD_VARARGS_END(Widget, tooltip, "takes no arguments or a single text string"),
//    Fl_Callback_p callback() const {return callback_;}
  { "callback", (PyCFunction)flpy_callback, METH_VARARGS },
//    void callback(Fl_Callback* cb, void* p) {callback_ = cb; user_data_ = p;}
//    void callback(Fl_Callback* cb) {callback_ = cb;}
//    void callback(Fl_Callback0* cb) {callback_ = (Fl_Callback*)cb;}
//    void callback(Fl_Callback1* cb, long p = 0) {
//    void* user_data() const {return user_data_;}
//    void user_data(void* v) {user_data_ = v;}
//    long argument() const {return (long)(fl_intptr_t)user_data_;}
//    void argument(long v) {user_data_ = (void*)(fl_intptr_t)v;}
  FlpyMETHOD_VARARGS_BEGIN(Widget, labelfont)
    FlpyARG_I_TO_v_TYPE(Widget, labelfont, Fl_Font)
    FlpyARG_v_TO_i(Widget, labelfont)
  FlpyMETHOD_VARARGS_END(Widget, labelfont, "takes no arguments or a font index"),
  FlpyMETHOD_VARARGS_BEGIN(Widget, when)
    FlpyARG_I_TO_v_TYPE(Widget, when, Fl_When)
    FlpyARG_v_TO_i(Widget, when)
  FlpyMETHOD_VARARGS_END(Widget, when, "takes no arguments or callback trigger flags"),
  FlpyMETHOD_v_TO_I(Widget, visible),
  FlpyMETHOD_v_TO_i(Widget, visible_r),
  FlpyMETHOD_VIRT_v_TO_v(Widget, show),
  FlpyMETHOD_VIRT_v_TO_v(Widget, hide),
  FlpyMETHOD_v_TO_v(Widget, set_visible),
  FlpyMETHOD_v_TO_v(Widget, clear_visible),
  FlpyMETHOD_v_TO_I(Widget, active),
  FlpyMETHOD_v_TO_i(Widget, active_r),
  FlpyMETHOD_v_TO_v(Widget, activate),
  FlpyMETHOD_v_TO_v(Widget, deactivate),
  FlpyMETHOD_v_TO_i(Widget, output),
  FlpyMETHOD_v_TO_v(Widget, set_output),
  FlpyMETHOD_v_TO_v(Widget, clear_output),
  FlpyMETHOD_v_TO_I(Widget, takesevents),
  FlpyMETHOD_v_TO_i(Widget, changed),
  FlpyMETHOD_v_TO_v(Widget, set_changed),
  FlpyMETHOD_v_TO_v(Widget, clear_changed),
  FlpyMETHOD_v_TO_v(Widget, set_active),
  FlpyMETHOD_v_TO_v(Widget, clear_active),
  FlpyMETHOD_v_TO_i(Widget, take_focus),
  FlpyMETHOD_v_TO_v(Widget, set_visible_focus),
  FlpyMETHOD_v_TO_v(Widget, clear_visible_focus),
  FlpyMETHOD_VARARGS_BEGIN(Widget, visible_focus)
    FlpyARG_i_TO_v(Widget, visible_focus)
    FlpyARG_v_TO_i(Widget, visible_focus)
  FlpyMETHOD_VARARGS_END(Widget, visible_focus, "takes no arguments or a text size"),
//    void do_callback(Fl_Callback_Reason reason=FL_REASON_UNKNOWN) {do_callback(this, user_data_, reason);}
//    void do_callback(Fl_Widget *widget, long arg, Fl_Callback_Reason reason=FL_REASON_UNKNOWN) {
//    void do_callback(Fl_Widget *widget, void *arg = 0, Fl_Callback_Reason reason=FL_REASON_UNKNOWN);
  FlpyMETHOD_v_TO_i(Widget, test_shortcut),
//    void _set_fullscreen() {flags_ |= FULLSCREEN;}
//    void _clear_fullscreen() {flags_ &= ~FULLSCREEN;}
  FlpyMETHOD_VARARGS_BEGIN(Widget, contains)
    FlpyARG_w_TO_i(Widget, contains)
  FlpyMETHOD_VARARGS_END(Widget, contains, "takes single widget argument"),
  FlpyMETHOD_VARARGS_BEGIN(Widget, inside)
    FlpyARG_w_TO_i(Widget, inside)
  FlpyMETHOD_VARARGS_END(Widget, inside, "takes single widget argument"),
  FlpyMETHOD_v_TO_v(Widget, redraw),
  FlpyMETHOD_v_TO_v(Widget, redraw_label),
  FlpyMETHOD_I0_TO_v(Widget, clear_damage),
  FlpyMETHOD_VARARGS_BEGIN(Widget, damage)
    FlpyARG_v_TO_i(Widget, damage)
    FlpyARG_I_TO_v_TYPE(Widget, damage, uchar)
    FlpyARG_Iiiii_TO_v_TYPE(Widget, damage, uchar)
  FlpyMETHOD_VARARGS_END(Widget, damage, "takes no argument, or damage flags and optional box dimensions"),
  FlpyMETHOD_VARARGS_BEGIN(Widget, draw_label)
    FlpyARG_iiiiI_TO_v_TYPE(Widget, draw_label, Fl_Align)
  FlpyMETHOD_VARARGS_END(Widget, draw_label, "takes box dimensions and an aligment"),
  { "measure_label", (PyCFunction)flpy_measure_label, METH_VARARGS },
  FlpyMETHOD_v_TO_w(Widget, window),
  FlpyMETHOD_v_TO_w(Widget, top_window),
  { "top_window_offset", (PyCFunction)flpy_top_window_offset, METH_NOARGS },
  FlpyMETHOD_v_TO_w(Widget, as_group),
  FlpyMETHOD_v_TO_w(Widget, as_window),
  FlpyMETHOD_v_TO_w(Widget, as_gl_window),
  FlpyMETHOD_v_TO_i(Widget, use_accents_menu),
  FlpyMETHOD_VARARGS_BEGIN(Widget, color2)
    FlpyARG_I_TO_v_TYPE(Widget, color2, Fl_Color)
    FlpyARG_v_TO_i(Widget, color2)
  FlpyMETHOD_VARARGS_END(Widget, color2, "takes no arguments or one color"),
  FlpyMETHOD_VARARGS_BEGIN(Widget, shortcut_label)
    FlpyARG_i_TO_v(Widget, shortcut_label)
    FlpyARG_v_TO_i(Widget, shortcut_label)
  FlpyMETHOD_VARARGS_END(Widget, shortcut_label, "takes no arguments or one integer"),
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

