//
// Callback manager header file for the Fast Light Tool Kit (FLTK).
//
// Copyright 2025 by Bill Spitzak and others.
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

/** \file FL/Fl_Callback.H
  \brief FFLTK Callback Helper
*/

#ifndef Fl_Callback_H
#define Fl_Callback_H

#include <functional>


/** A class prototype that allows for additional data in callbacks.

 Users can derive this class and pass objects of such derived classes to widget callbacks.
 Widgets can take ownership of the callback data, deleting the data when the widget
 itself is deleted.

 The destructor of this class is virtual, allowing for additional code to
 deallocate resources when the user data is deleted.

 \see FL_FUNCTION_CALLBACK_3, FL_METHOD_CALLBACK_1, FL_INLINE_CALLBACK_2
 \see Fl_Widget::callback(Fl_Callback*, Fl_Callback_User_Data*, bool)
 \see Fl_Widget::user_data(Fl_Callback_User_Data*, bool)
 */
class FL_EXPORT Fl_Callback_User_Data {
protected:
  Fl_Callback_User_Data() {} ///< Protected constructor
public:
  virtual ~Fl_Callback_User_Data() { } ///< Destructor
};


/** Default callback type definition for all fltk widgets (by far the most used) */
typedef void (Fl_Callback )(Fl_Widget*, void*);

/** Default callback type pointer definition for all fltk widgets */
typedef Fl_Callback* Fl_Callback_p; // needed for BORLAND

/** One parameter callback type definition passing only the widget */
typedef void (Fl_Callback0)(Fl_Widget*);

/** Callback type definition passing the widget and a long data value */
typedef void (Fl_Callback1)(Fl_Widget*, long);

/** Callback type definition passing the widget and Fl_Callback_User_Data */
typedef void (Fl_CallbackUD)(Fl_Widget*, Fl_Callback_User_Data*);

/** C++11 callback type */
typedef std::function<void(Fl_Widget*)> Fl_CallbackStdFn;


/**
 Core implementation of C++98 and C++11 and higher callbacks.
 */
class Fl_Callback_Base {
public:
  enum class CallbackType : unsigned {
    VOID_PTR, LONG, VOID, DATA_PTR, MANAGED_PTR, STD_FN
  };

  template<class A, class B>
  static void clear_user_data(A *a, B *b) {
    if (b->callback_type_ == CallbackType::MANAGED_PTR) {
      if (a->callback_user_data_) {
        delete a->callback_user_data_;
        a->callback_user_data_ = nullptr;
      }
    }
  }

  template<class A, class B>
  static void set_callback_type(A *a, B *b, CallbackType new_t) {
    CallbackType old_t = b->callback_type_;
    if (new_t == old_t) return;
    switch (old_t) {
      case CallbackType::STD_FN: a->callback_std_fn_ = nullptr; break;
      case CallbackType::MANAGED_PTR: clear_user_data(a, b); break;
      default: break;
    }
    switch (new_t) {
      case CallbackType::VOID_PTR:
        a->callback_ = nullptr; a->user_data_ = nullptr; break;
      case CallbackType::LONG:
        a->callback1_ = nullptr; a->user_data1_ = 0; break;
      case CallbackType::VOID:
        a->callback0_ = nullptr; break;
      case CallbackType::DATA_PTR:
      case CallbackType::MANAGED_PTR:
        a->callback_ud_ = nullptr; a->callback_user_data_ = nullptr; break;
      case CallbackType::STD_FN:
        new (&a->callback_) std::function<void(Fl_Widget*)>(nullptr);
    }
    b->callback_type_ = new_t;
  }

  template<class A, class B>
  static Fl_Callback_p get_callback(A *a, B *b) {
    if (b->callback_type_ == CallbackType::STD_FN) {
      return nullptr;
    } else {
      return a->callback_;
    }
  }

  template<class A, class B>
  static void set_callback(A *a, B *b, Fl_Callback *cb, void *p) {
    set_callback_type(a, b, CallbackType::VOID_PTR);
    a->callback_ = cb;
    a->user_data_ = p;
  }

  template<class A, class B>
  static void set_callback(A *a, B *b, Fl_Callback1 *cb, long p) {
    set_callback_type(a, b, CallbackType::LONG);
    a->callback1_ = cb;
    a->user_data1_ = p;
  }

  template<class A, class B>
  static void set_callback(A *a, B *b, Fl_Callback *cb, Fl_Callback_User_Data* p, bool auto_free) {
    set_callback_type(a, b, auto_free ? CallbackType::MANAGED_PTR : CallbackType::DATA_PTR);
    a->callback_ud_ = reinterpret_cast<Fl_CallbackUD*>(cb);
    if (a->callback_user_data_ != p) clear_user_data(a, b);
    a->callback_user_data_ = p;
  }

  template<class A, class B>
  static void set_callback(A *a, B *b, Fl_CallbackStdFn cb) {
    set_callback_type(a, b, CallbackType::STD_FN);
    a->callback_std_fn_ = std::move(cb);
  }

  template<class A, class B>
  static void *get_user_data(A *a, B *b) {
    switch (b->callback_type_) {
      case CallbackType::VOID_PTR: return a->user_data_;
      case CallbackType::LONG: return reinterpret_cast<void*>(a->user_data1_);
      case CallbackType::STD_FN:
      case CallbackType::VOID: return nullptr;
      case CallbackType::DATA_PTR:
      case CallbackType::MANAGED_PTR: return static_cast<void*>(a->callback_user_data_);
    }
  }

  template<class A, class B>
  static void set_user_data(A *a, B *b, void *v, bool managed) {
    switch (b->callback_type_) {
      case CallbackType::VOID_PTR: a->user_data_ = v; break;
      case CallbackType::LONG: a->user_data1_ = reinterpret_cast<long>(v); break;
      case CallbackType::DATA_PTR:
        if (managed) b->callback_type_ = CallbackType::MANAGED_PTR;
        a->callback_user_data_ = reinterpret_cast<Fl_Callback_User_Data*>(v); break;
      case CallbackType::MANAGED_PTR:
        clear_user_data(a, b);
        if (!managed) b->callback_type_ = CallbackType::DATA_PTR;
        a->callback_user_data_ = reinterpret_cast<Fl_Callback_User_Data*>(v); break;
      case CallbackType::STD_FN:
      case CallbackType::VOID: break;
    }
  }

  template<class A, class B>
  static long get_argument(A *a, B *b) {
    switch (b->callback_type_) {
      case CallbackType::VOID_PTR: return reinterpret_cast<long>(a->user_data_);
      case CallbackType::LONG: return a->user_data1_;
      case CallbackType::STD_FN:
      case CallbackType::VOID: return 0;
      case CallbackType::DATA_PTR:
      case CallbackType::MANAGED_PTR: return reinterpret_cast<long>(a->callback_user_data_);
    }
  }

  template<class A, class B>
  static void set_argument(A *a, B *b, long v) {
    switch (b->callback_type_) {
      case CallbackType::VOID_PTR: a->user_data_ = reinterpret_cast<void*>(v); break;
      case CallbackType::LONG: a->user_data1_ = v; break;
      case CallbackType::MANAGED_PTR:
        clear_user_data(a, b);
        b->callback_type_ = CallbackType::DATA_PTR;
        /* fallthrough */
      case CallbackType::DATA_PTR:
        a->callback_user_data_ = reinterpret_cast<Fl_Callback_User_Data*>(v); break;
      case CallbackType::STD_FN:
      case CallbackType::VOID: break;
    }
  }

  template<class A, class B>
  static bool do_callback(A *a, B *b, Fl_Widget *w, uint8_t sel, void *vp, long lg) {
    switch (b->callback_type_) {
      case CallbackType::VOID_PTR: {
        if (!a->callback_) return false;
        void *ud;
        if (sel == 0) ud = a->user_data_;
        else if (sel == 1) ud = vp;
        else ud = reinterpret_cast<void*>(lg);
        a->callback_(w, ud);
      } break;
      case CallbackType::LONG: {
        if (!a->callback1_) return false;
        long ud;
        if (sel == 0) ud = a->user_data1_;
        else if (sel == 1) ud = reinterpret_cast<long>(vp);
        else ud = lg;
        a->callback1_(w, ud);
      } break;
      case CallbackType::VOID:
        if (!a->callback0_) return false;
        a->callback0_(w);
        break;
      case CallbackType::DATA_PTR:
      case CallbackType::MANAGED_PTR: {
        if (!a->callback_ud_) return false;
        Fl_Callback_User_Data *ud;
        if (sel == 0) ud = a->callback_user_data_;
        else if (sel == 1) ud = reinterpret_cast<Fl_Callback_User_Data*>(vp);
        else ud = reinterpret_cast<Fl_Callback_User_Data*>(lg);
        a->callback_ud_(w, ud);
      } break;
      case CallbackType::STD_FN:
        if (!a->callback_std_fn_) return false;
        a->callback_std_fn_(w);
        break;
    }
    return true;
  }
};

class Fl_Callback_Core : public Fl_Callback_Base {
  friend Fl_Callback_Base;
protected:
  union {
    struct {
      Fl_Callback* callback_;
      void* user_data_;
    };
    struct {
      Fl_Callback1* callback1_;
      intptr_t user_data1_;
    };
    Fl_Callback0* callback0_;
    struct {
      Fl_CallbackUD* callback_ud_;
      Fl_Callback_User_Data *callback_user_data_;
    };
    Fl_CallbackStdFn callback_std_fn_;
  };
public:
  Fl_Callback_Core() {}
  ~Fl_Callback_Core() {} // user must call set_callback_type() to destroy
};

#if 0
class Fl_Callback_Manager : public Fl_Callback_Core {
  friend class Fl_Callback_Core;
protected:
  CallbackType callback_type_ : 8;
public:
  Fl_Callback_p get_callback() const {
    return Fl_Callback_Core::get_callback(this);
  }
  void set_callback() {
    this->callback_ = nullptr;
    this->callback_type_ = CallbackType::VOID_PTR;
  };
};
#endif

#endif // !Fl_Callback_H
