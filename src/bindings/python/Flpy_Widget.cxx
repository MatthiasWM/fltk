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


/*
 Flpy_Widget is the base class for all widget types.

 Creating a widget will possibly auto-add it to the current group before the
 PyObject reference is set. Deleting a group widget must not delete the child
 widgets, but instead decrement their Python reference.

 To allow cycloc GC, we must handle all possible obvious and hidden refs.
 Obvious refs are in FlpyObject_HEAD. The hidden ones are refs to image and
 deimge, and to all children of group widgets. Some widget types may reference
 "internal" widgets like scrollbars.
 */

// The Flpy_ class allows us to access protected members
class Flpy_Widget;
// The Flpy_Derived_ class allows users to override virtual methods
class Flpy_Derived_Widget;

// Python 'self' representation of an FLTK widget, user_data() links here
typedef struct {
  FlpyObject_HEAD     // Data needed for every widget that has no room in the FLTK class
  Flpy_Widget *o;     // Link from 'self' back to widget
} Flpy_Widget_Object;

// In C++ we can't instantiate Fl_Widget. To make that easier in Python, we derive from Fl_Box
class Flpy_Widget : public Fl_Box {
public:
  Flpy_Widget(int x, int y, int w, int h) : Fl_Box(x, y, w, h) { }

  // the following members are called from Pythin directly
  static int flpy_init(Flpy_Widget_Object *self, PyObject *args, PyObject*);
  static int flpy_traverse(Flpy_Widget_Object *self, visitproc visit, void *arg);
  static int flpy_clear(Flpy_Widget_Object *self);
  static void flpy_dealloc(Flpy_Widget_Object *self);
  static PyMethodDef flpy_methods[];
  static PyGetSetDef flpy_getset[];

  static PyObject *flpy_callback(Flpy_Widget_Object *self, PyObject *args) {
    if (PyTuple_Size(args) == 0) {
      if (self->callback_method) return self->callback_method;
      Py_RETURN_NONE;
    } else {
      PyObject *method = NULL;
      PyObject *data = NULL;
      if (!PyArg_ParseTuple(args, "O|O", &method, &data))
        return NULL;
      Py_XINCREF(method);
      Py_XINCREF(data);
      PyObject *method_bak = self->callback_method;
      PyObject *data_bak = self->callback_data;
      self->callback_method = NULL;
      self->callback_data = NULL;
      Py_XDECREF(method_bak);
      Py_XDECREF(data_bak);
      self->callback_method = method;
      self->callback_data = data;
      Py_RETURN_NONE;
    }
    return NULL;
  }
  // More complex method that is not predefined, optional arguments, returns a tuple
  static PyObject *flpy_measure_label(Flpy_Widget_Object *self, PyObject *args) {
    int arg0 = 0, arg1 = 0;
    if (!PyArg_ParseTuple(args, "|i", &arg0)) return NULL;
    self->o->measure_label(arg0, arg1);
    return Py_BuildValue("(ii)", arg0, arg1);
  }
  // More complex method that is not predefined, returns a tuple
  static PyObject *flpy_top_window_offset(Flpy_Widget_Object *self, PyObject *args) {
    int arg0 = 0, arg1 = 0;
    Fl_Window *win = self->o->top_window_offset(arg0, arg1);
    PyObject *win_obj = win ? (PyObject*)win->user_data() : Py_None ;
    return Py_BuildValue("(Oii)", win_obj, arg0, arg1);
  }
};

class Flpy_Derived_Widget : public Flpy_Widget {
public:
  Flpy_Derived_Widget(int x, int y, int w, int h) : Flpy_Widget(x, y, w, h) { }
  // TODO: void show()
  // TODO: void hide()
  void draw() FL_OVERRIDE {
    Flpy_Derived_Widget *self = (Flpy_Derived_Widget*)user_data();
    PyObject_CallMethodNoArgs((PyObject*)self, PyUnicode_FromString("draw"));
  }
  // TODO: int handle(e)
  // TODO: void resize(x, y, w, h)
};


// TODO: roll this into a macro because it's the same for every widget class
// TODO: we should move this into __new__ where we already have all the parameters, so the user can override __init__
int Flpy_Widget::flpy_init(Flpy_Widget_Object *self, PyObject *args, PyObject*) {
  int x, y, w, h;
  char *label_ = NULL;
  if (!PyArg_ParseTuple(args, "iiii|z", &x, &y, &w, &h, &label_)) return -1;
  if (Py_TYPE(self) == &flpy_type)
    self->o = new Flpy_Widget(x, y, w, h);
  else
    self->o = new Flpy_Derived_Widget(x, y, w, h);
  Flpy_Widget *o = self->o;
  if (label_) o->copy_label(label_);
  o->callback(flpy_on_callback, self);
  if (o->parent()) Py_INCREF(self);
  return 0;
}

int Flpy_Widget::flpy_traverse(Flpy_Widget_Object *self, visitproc visit, void *arg)
{
  Py_VISIT(self->callback_method);
  Py_VISIT(self->callback_data);
  // FIXME: image
  // FIXME: deimage
  // FIXME: children
  return 0;
}

int Flpy_Widget::flpy_clear(Flpy_Widget_Object *self)
{
  Py_CLEAR(self->callback_method);
  Py_CLEAR(self->callback_data);
  // FIXME: image
  // FIXME: deimage
  // FIXME: children
  delete self->o;
  return 0;
}

void Flpy_Widget::flpy_dealloc(Flpy_Widget_Object *self) {
  PyObject_GC_UnTrack((PyObject*)self);
  flpy_clear(self);
  Py_TYPE(self)->tp_free(self);
}

// List of all class methods and their implementation with lambdas
PyMethodDef Flpy_Widget::flpy_methods[] = {
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
  FlpyMETHOD_VARARGS_END(Widget, draw_focus, "takes no arguments, or a boxtype, a box, and an optional color"),
  FlpyMETHOD_VARARGS_BEGIN(Widget, draw_label)
    FlpyARG_v_TO_v(Widget, draw_label)
    FlpyARG_iiii_TO_v(Widget, draw_label)
  FlpyMETHOD_VARARGS_END(Widget, draw_label, "takes no arguments, or box dimensions"),
  FlpyMETHOD_VIRT_v_TO_v(Widget, draw),
  FlpyMETHOD_VIRT_i_TO_i(Widget, handle),
  FlpyMETHOD_VARARGS_BEGIN(Widget, parent)
    FlpyARG_v_TO_w(Widget, parent)
    //FlpyARG_g_TO_v(Widget, parent) // not available
  FlpyMETHOD_VARARGS_END(Widget, parent, "takes no argument"),
  FlpyMETHOD_VARARGS_BEGIN(Widget, type)
    FlpyARG_v_TO_i(Widget, type)
    FlpyARG_I_TO_v_TYPE(Widget, type, uchar)
  FlpyMETHOD_VARARGS_END(Widget, type, "takes no argument, or an unsigned integer"),
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
  // TODO: we must wrap Image types before we can link to them, remeber the Ref Count
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
  // TODO: how exactly do we want the callabcks to work in Python
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
  //    void do_callback(Fl_Callback_Reason reason=FL_REASON_UNKNOWN) {do_callback(this, user_data_, reason);}
  //    void do_callback(Fl_Widget *widget, long arg, Fl_Callback_Reason reason=FL_REASON_UNKNOWN) {
  //    void do_callback(Fl_Widget *widget, void *arg = 0, Fl_Callback_Reason reason=FL_REASON_UNKNOWN);
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
  FlpyMETHOD_v_TO_i(Widget, test_shortcut),
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

// Lst of all class constants
PyGetSetDef Flpy_Widget::flpy_getset[] = {
  Flpy_GET_INT(INACTIVE),
  Flpy_GET_INT(INVISIBLE),
  Flpy_GET_INT(OUTPUT),
  Flpy_GET_INT(NOBORDER),
  Flpy_GET_INT(FORCE_POSITION),
  Flpy_GET_INT(NON_MODAL),
  Flpy_GET_INT(SHORTCUT_LABEL),
  Flpy_GET_INT(CHANGED),
  Flpy_GET_INT(OVERRIDE),
  Flpy_GET_INT(VISIBLE_FOCUS),
  Flpy_GET_INT(COPIED_LABEL),
  Flpy_GET_INT(CLIP_CHILDREN),
  Flpy_GET_INT(MENU_WINDOW),
  Flpy_GET_INT(TOOLTIP_WINDOW),
  Flpy_GET_INT(MODAL),
  Flpy_GET_INT(NO_OVERLAY),
  Flpy_GET_INT(GROUP_RELATIVE),
  Flpy_GET_INT(COPIED_TOOLTIP),
  Flpy_GET_INT(FULLSCREEN),
  Flpy_GET_INT(MAC_USE_ACCENTS_MENU),
  Flpy_GET_INT(NEEDS_KEYBOARD),
  Flpy_GET_INT(IMAGE_BOUND),
  Flpy_GET_INT(DEIMAGE_BOUND),
  Flpy_GET_INT(USERFLAG3),
  Flpy_GET_INT(USERFLAG2),
  Flpy_GET_INT(USERFLAG1),
  { NULL }
};

#if 0
FlpyTYPE(Widget, widget, NULL, "Fl_Widget is the base class for all widgets in FLTK.")
#else
PyTypeObject flpy_widget_type = {
  .ob_base = { PyObject_HEAD_INIT(NULL) },
  .tp_name = "fltk.Fl_Widget",
  .tp_basicsize = sizeof(Flpy_Widget_Object),
  .tp_itemsize = 0,
  .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_GC,
  .tp_doc = PyDoc_STR("Fl_Widget"),
  .tp_methods = Flpy_Widget::flpy_methods,
  .tp_getset = Flpy_Widget::flpy_getset,
  .tp_dealloc = (destructor)Flpy_Widget::flpy_dealloc,
  .tp_init = (initproc)Flpy_Widget::flpy_init,
  .tp_new = PyType_GenericNew,
  .tp_traverse = (traverseproc)Flpy_Widget::flpy_traverse,
  .tp_clear = (inquiry)Flpy_Widget::flpy_clear,
};
#endif

// TODO: the callbacks of all widgets lead here, move this to Flpy.cxx
// TODO: and add handling of different callback reasons
void flpy_on_callback(Fl_Widget *w, void *v) {
  Flpy_Widget_Object *self = (Flpy_Widget_Object*)v;
  if (!self) return;
  if (self->callback_method && !Py_IsNone(self->callback_method)) {
    PyObject *data = self->callback_data;
    if (!data) data = Py_None;
    PyObject *tuple = PyTuple_Pack(2, self, data);
    Py_INCREF(tuple);
    PyObject *result = PyObject_CallObject(self->callback_method, tuple);
    Py_DECREF(tuple);
    Py_XDECREF(result);
  } else if (PyObject_HasAttrString((PyObject*)self, "on_callback")) {
    PyObject *data = self->callback_data;
    if (!data) data = Py_None;
    PyObject *result = PyObject_CallMethodOneArg((PyObject*)self, PyUnicode_FromString("on_callback"), data);
    Py_XDECREF(result);
  }
  // TODO: we could also test if self.on_callback exists and call that. Would that be easier?
}

