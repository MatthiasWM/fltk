//
// Menu window code for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2025 by Bill Spitzak and others.
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

// This is the window type used by Fl_Menu to make the pop-ups.
// It draws in the overlay planes if possible.

// Also here is the implementation of the mouse & keyboard grab,
// which are used so that clicks outside the program's windows
// can be used to dismiss the menus.

#include <FL/Fl_Menu_Window.H>
#include <FL/Fl_Menu_.H>
#include "Fl_Window_Driver.H"


// We can keep the pointer in a list and don't have to watch out for deleted
// windows, because deleting a window would first hide it which removes it
// from this set.
std::list<Fl_Menu_Window*> Fl_Menu_Window::open_windows_;

void Fl_Menu_Window::register_open_window_(Fl_Menu_Window *win) {
  open_windows_.remove(win); // 
  open_windows_.insert(win);
  if (open_windows_.size() == 1)
    Fl::add_handler(app_event_handler_);
}

void Fl_Menu_Window::deregister_closed_window_(Fl_Menu_Window *win) {
  open_windows_.erase(win);
  if (open_windows_.size() == 0)
    Fl::remove_handler(app_event_handler_);
}

int Fl_Menu_Window::app_event_handler_(int event) {
  // TODO: If the app loses focus, all popup menus should close
  // TODO: If we click outside the topmost window, that window (or all windows) should close
  for (auto w: open_windows_) {
  }
}


/**  Destroys the window and all of its children.*/
Fl_Menu_Window::~Fl_Menu_Window() {
  hide();
}


Fl_Menu_Window::Fl_Menu_Window(int W, int H, const char *l)
: super(W,H,l)
{
  image(0);
}


Fl_Menu_Window::Fl_Menu_Window(int X, int Y, int W, int H, const char *l)
: super(X,Y,W,H,l) {
  image(0);
}

static Fl_Group *ctor_current_group_trick_;

Fl_Menu_Window::Fl_Menu_Window()
: super(
                   // The trick below saves the current widget before any
                   // superclass ctors can be called:
                   ( (ctor_current_group_trick_ = Fl_Group::current()), Fl_Group::current(0), 200),
                   200, 100, 10),
  ctor_current_group_ ( ctor_current_group_trick_ )
{
}

void Fl_Menu_Window::end() {
  Fl_Group::current( ctor_current_group_ );
}

void Fl_Menu_Window::show() {
  if (!shown())
    register_open_window_(this);
  super::show();
}

void Fl_Menu_Window::hide() {
  if (shown())
    deregister_closed_window_(this);
  super::hide();
}

void Fl_Menu_Window::init() {
  if (initialized_) return;
  initialized_ = true;
//  menu = m;
  set_menu_window();
  Fl_Window_Driver::driver(this)->set_popup_window();
//  end();
  set_modal();
  clear_border();
}


void Fl_Menu_Window::init_position(const Fl_Menu_* pbutton) {
//  int scr_x, scr_y, scr_w, scr_h;
//  int tx = X, ty = Y;
//  menubartitle = menubar_title;
//  origin = NULL;
//  offset_y = 0;
//  int n = (Wp > 0 ? Fl::screen_num(X, Y) : -1);
//  Fl_Window_Driver::driver(this)->menu_window_area(scr_x, scr_y, scr_w, scr_h, n);
//  if (!right_edge || right_edge > scr_x+scr_w) right_edge = scr_x+scr_w;
//
//  if (m) m = m->first(); // find the first item that needs to be rendered
//  drawn_selected = -1;
//  if (button) {
//    Fl_Boxtype b = button->menu_box();
//    if (b==FL_NO_BOX)
//      b = button->box();
//    if (b==FL_NO_BOX)
//      b = FL_FLAT_BOX;
//    box(b);
//  } else {
//    box(FL_UP_BOX);
//  }
//  color(button && !Fl::scheme() ? button->color() : FL_GRAY);
//  selected = -1;
//  {
//    int j = 0;
//    if (m) for (const Fl_Menu_Item* m1=m; ; m1 = m1->next(), j++) {
//      if (picked) {
//        if (m1 == picked) {selected = j; picked = 0;}
//        else if (m1 > picked) {selected = j-1; picked = 0; Wp = Hp = 0;}
//      }
//      if (!m1->text) break;
//    }
//    numitems = j;}
//
//  if (menubar) {
//    itemheight = 0;
//    title = 0;
//    return;
//  }
//
//  itemheight = 1;
//
//  int hotKeysw = 0;
//  int hotModsw = 0;
//  int Wtitle = 0;
//  int Htitle = 0;
//  if (t) Wtitle = t->measure(&Htitle, button) + 12;
//  int W = 0;
//  if (m) for (; m->text; m = m->next()) {
//    int hh;
//    int w1 = m->measure(&hh, button);
//    if (hh+Fl::menu_linespacing()>itemheight) itemheight = hh+Fl::menu_linespacing();
//    if (m->flags&(FL_SUBMENU|FL_SUBMENU_POINTER))
//      w1 += FL_NORMAL_SIZE;
//    if (w1 > W) W = w1;
//    // calculate the maximum width of all shortcuts
//    if (m->shortcut_) {
//      // s is a pointer to the UTF-8 string for the entire shortcut
//      // k points only to the key part (minus the modifier keys)
//      const char *k, *s = fl_shortcut_label(m->shortcut_, &k);
//      if (fl_utf_nb_char((const unsigned char*)k, (int) strlen(k))<=4) {
//        // a regular shortcut has a right-justified modifier followed by a left-justified key
//        w1 = int(fl_width(s, (int) (k-s)));
//        if (w1 > hotModsw) hotModsw = w1;
//        w1 = int(fl_width(k))+4;
//        if (w1 > hotKeysw) hotKeysw = w1;
//      } else {
//        // a shortcut with a long modifier is right-justified to the menu
//        w1 = int(fl_width(s))+4;
//        if (w1 > (hotModsw+hotKeysw)) {
//          hotModsw = w1-hotKeysw;
//        }
//      }
//    }
//  }
//  shortcutWidth = hotKeysw;
//  if (selected >= 0 && !Wp) X -= W/2;
//  int BW = Fl::box_dx(box());
//  W += hotKeysw+hotModsw+2*BW+7;
//  if (Wp > W) W = Wp;
//  if (Wtitle > W) W = Wtitle;
//
//  if (X < scr_x) X = scr_x;
//  // this change improves popup submenu positioning at right screen edge,
//  // but it makes right_edge argument useless
//  //if (X > scr_x+scr_w-W) X = right_edge-W;
//  if (X > scr_x+scr_w-W) X = scr_x+scr_w-W;
//  x(X); w(W);
//  h((numitems ? itemheight*numitems-4 : 0)+2*BW+3);
//  if (selected >= 0) {
//    Y = Y+(Hp-itemheight)/2-selected*itemheight-BW;
//  } else {
//    Y = Y+Hp;
//    // if the menu hits the bottom of the screen, we try to draw
//    // it above the menubar instead. We will not adjust any menu
//    // that has a selected item.
//    if (Y+h()>scr_y+scr_h && Y-h()>=scr_y) {
//      if (Hp>1) {
//        // if we know the height of the Fl_Menu_, use it
//        Y = Y-Hp-h();
//      } else if (t) {
//        // assume that the menubar item height relates to the first
//        // menuitem as well
//        Y = Y-itemheight-h()-Fl::box_dh(box());
//      } else {
//        // draw the menu to the right
//        Y = Y-h()+itemheight+Fl::box_dy(box());
//      }
//      if (t) {
//        if (menubar_title) {
//          Y = Y + Fl::menu_linespacing() - Fl::box_dw(button->box());
//        } else {
//          Y += 2*Htitle+2*BW+3;
//        }
//      }
//    }
//  }
//  if (m) y(Y); else {y(Y-2); w(1); h(1);}
//
//  if (t) {
//    if (menubar_title) {
//      int dy = Fl::box_dy(button->box())+1;
//      int ht = button->h()-dy*2;
//      title = new menutitle(tx, ty-ht-dy, Wtitle, ht, t, true);
//    } else {
//      int dy = 2;
//      int ht = Htitle+2*BW+3;
//      title = new menutitle(X, Y-ht-dy, Wtitle, ht, t);
//    }
//  } else {
//    title = 0;
//  }
}

void Fl_Menu_Window::layout() {
  // TODO: MATT: Send an event to all children FL_MENU_COLLECT_SIZES_EVENT
  // Calculate all sizes for the children and for this window
  // Resize the window (without resizing the children).
  // Resize and reposition all children by calling resize()
  // and sending FL_MENU_SET_SIZES_EVENT for optional detailed information
  // (label offsets, shortcut position, etc.). This event will also put the
  // widget into Menu mode if available.
}

/**
  Pulldown() is similar to popup(), but a rectangle is provided
  to position the menu.

  The menu is made at least \p W wide, and the picked item \p initial_item
  is centered over the rectangle (like Fl_Choice uses).

  If \p initial_item is \p NULL or not found, the menu is aligned just
  below the rectangle (like a pulldown menu).

  The \p title and \p menubar arguments are used internally by the
  Fl_Menu_Bar widget.
*/
void Fl_Menu_Window::pulldown(
    int X, int Y, int W, int H,
    const Fl_Menu_Item* initial_item,
    const Fl_Menu_* pbutton,
    const Fl_Menu_Item* title,
    int menubar)
{
  init();
//  Fl_Group::current(0); // fix possible user error...
//
//  // track the Fl_Menu_ widget to make sure we notice if it gets
//  // deleted while the menu is open (STR #3503)
  Fl_Widget_Tracker wp((Fl_Widget *)pbutton);
//
//  button = pbutton;
//  if (pbutton && pbutton->window()) {
//    menuwindow::parent_ = pbutton->top_window();
//    for (Fl_Window* w = pbutton->window(); w; w = w->window()) {
//      X += w->x();
//      Y += w->y();
//    }
//  } else {
//    X += Fl::event_x_root()-Fl::event_x();
//    Y += Fl::event_y_root()-Fl::event_y();
//    menuwindow::parent_ = Fl::first_window();
//  }
//
//  int XX, YY, WW;
//  Fl::screen_xywh(XX, YY, WW, menuwindow::display_height_, menuwindow::parent_->screen_num());
//  menuwindow mw(this, X, Y, W, H, initial_item, title, menubar);
//  Fl::grab(mw);
//  // If we grab the mouse pointer, we should also make sure that it is visible.
//  if (menuwindow::parent_)
//    menuwindow::parent_->cursor(FL_CURSOR_DEFAULT);
//  menustate pp; p = &pp;
//  pp.p[0] = &mw;
//  pp.nummenus = 1;
//  pp.menubar = menubar;
//  pp.state = INITIAL_STATE;
//  pp.fakemenu = 0; // kludge for buttons in menubar
//
//  // preselected item, pop up submenus if necessary:
//  if (initial_item && mw.selected >= 0) {
//    setitem(0, mw.selected);
//    goto STARTUP;
//  }
//
//  pp.current_item = 0; pp.menu_number = 0; pp.item_number = -1;
//  if (menubar) {
//    // find the initial menu
//    if (!mw.handle(FL_DRAG)) {
//      Fl::grab(0);
//      return 0;
//    }
//  }
//  initial_item = pp.current_item;
//  if (initial_item) {
//    if (menubar && !initial_item->activevisible()) { // pointing at inactive item
//      Fl::grab(0);
//      return NULL;
//    }
//    goto STARTUP;
//  }
//
  
  show();
  Fl::grab(this);
  // TODO: MATT: we don't need our own event loop.
  // All widgets can take care of themselves, and this window is modal, so no
  // other window will get events until we are done here.

//  // the main loop: runs until p.state goes to DONE_STATE or the menu
//  // widget is deleted (e.g. from a timer callback, see STR #3503):
//  for (;;) {
//
//    // make sure all the menus are shown:
//    {
//      for (int k = menubar; k < pp.nummenus; k++) {
//        if (!pp.p[k]->shown()) {
//          if (pp.p[k]->title) pp.p[k]->title->show();
//          pp.p[k]->show();
//        }
//      }
//    }
//
//    // get events:
//    {
//      const Fl_Menu_Item* oldi = pp.current_item;
//      Fl::wait();
//      if (pbutton && wp.deleted()) // menu widget has been deleted (STR #3503)
//        break;
//      if (pp.state == DONE_STATE) break; // done.
//      if (pp.current_item == oldi) continue;
//    }
//
//    // only do rest if item changes:
//    if(pp.fakemenu) {delete pp.fakemenu; pp.fakemenu = 0;} // turn off "menubar button"
//
//    if (!pp.current_item) { // pointing at nothing
//      // turn off selection in deepest menu, but don't erase other menus:
//      pp.p[pp.nummenus-1]->set_selected(-1);
//      continue;
//    }
//
//    if(pp.fakemenu) {delete pp.fakemenu; pp.fakemenu = 0;}
//    initial_item = 0; // stop the startup code
//    pp.p[pp.menu_number]->autoscroll(pp.item_number);
//
//  STARTUP:
//    menuwindow& cw = *pp.p[pp.menu_number];
//    const Fl_Menu_Item* m = pp.current_item;
//    if (!m->activevisible()) { // pointing at inactive item
//      cw.set_selected(-1);
//      initial_item = 0; // turn off startup code
//      continue;
//    }
//    cw.set_selected(pp.item_number);
//
//    if (m==initial_item) initial_item=0; // stop the startup code if item found
//    if (m->submenu()) {
//      const Fl_Menu_Item* title = m;
//      const Fl_Menu_Item* menutable;
//      if (m->flags&FL_SUBMENU) menutable = m+1;
//      else menutable = (Fl_Menu_Item*)(m)->user_data_;
//      // figure out where new menu goes:
//      int nX, nY;
//      if (!pp.menu_number && pp.menubar) {      // menu off a menubar:
//        nX = cw.x() + cw.titlex(pp.item_number);
//        nY = cw.y() + cw.h();
//        initial_item = 0;
//      } else {
//        nX = cw.x() + cw.w();
//        nY = cw.y() + pp.item_number * cw.itemheight;
//        title = 0;
//      }
//      if (initial_item) { // bring up submenu containing initial item:
//        menuwindow* n = new menuwindow(menutable,X,Y,W,H,initial_item,title,0,0,cw.x());
//        pp.p[pp.nummenus++] = n;
//        if (pp.nummenus >= 2) pp.p[pp.nummenus-1]->origin = pp.p[pp.nummenus-2];
//        // move all earlier menus to line up with this new one:
//        if (n->selected>=0) {
//          int dy = n->y()-nY;
//          int dx = n->x()-nX;
//          int waX, waY, waW, waH;
//          Fl_Window_Driver::driver(n)->menu_window_area(waX, waY, waW, waH, Fl::screen_num(X, Y));
//          for (int menu = 0; menu <= pp.menu_number; menu++) {
//            menuwindow* tt = pp.p[menu];
//            int nx = tt->x()+dx; if (nx < waX) {nx = waX; dx = -tt->x() + waX;}
//            int ny = tt->y()+dy; if (ny < waY) {ny = waY; dy = -tt->y() + waY;}
//            tt->position(nx, ny);
//          }
//          setitem(pp.nummenus-1, n->selected);
//          goto STARTUP;
//        }
//      } else if (pp.nummenus > pp.menu_number+1 &&
//                 pp.p[pp.menu_number+1]->menu == menutable) {
//        // the menu is already up:
//        while (pp.nummenus > pp.menu_number+2) delete pp.p[--pp.nummenus];
//        pp.p[pp.nummenus-1]->set_selected(-1);
//      } else {
//        // delete all the old menus and create new one:
//        while (pp.nummenus > pp.menu_number+1) delete pp.p[--pp.nummenus];
//        pp.p[pp.nummenus++]= new menuwindow(menutable, nX, nY,
//                                          title?1:0, 0, 0, title, 0, menubar,
//                                            (title ? 0 : cw.x()) );
//        if (pp.nummenus >= 2 && pp.p[pp.nummenus-2]->itemheight) {
//          pp.p[pp.nummenus-1]->origin = pp.p[pp.nummenus-2];
//        }
//      }
//    } else { // !m->submenu():
//      while (pp.nummenus > pp.menu_number+1) delete pp.p[--pp.nummenus];
//      if (!pp.menu_number && pp.menubar) {
//        // kludge so "menubar buttons" turn "on" by using menu title:
//        pp.fakemenu = new menuwindow(0,
//                                  cw.x()+cw.titlex(pp.item_number),
//                                  cw.y()+cw.h(), 0, 0,
//                                  0, m, 0, 1);
//        pp.fakemenu->title->show();
//      }
//    }
//  }
//  const Fl_Menu_Item* m = (pbutton && wp.deleted()) ? NULL : pp.current_item;
//  delete pp.fakemenu;
//  while (pp.nummenus>1) delete pp.p[--pp.nummenus];
//  mw.hide();
//  Fl::grab(0);
//  menuwindow::parent_ = NULL;
//  return m;
}

int Fl_Menu_Window::handle(int e) {
//  menustate &pp = *p;
  switch (e) {
    case FL_KEYBOARD:
      switch (Fl::event_key()) {
//        case FL_BackSpace:
//        BACKTAB:
//          backward(pp.menu_number);
//          return 1;
//        case FL_Up:
//          if (pp.menubar && pp.menu_number == 0) {
//            // Do nothing...
//          } else if (backward(pp.menu_number)) {
//            // Do nothing...
//          } else if (pp.menubar && pp.menu_number==1) {
//            setitem(0, pp.p[0]->selected);
//          }
//          return 1;
//        case FL_Tab:
//          if (Fl::event_shift()) goto BACKTAB;
//          if (pp.menubar && pp.menu_number == 0) goto RIGHT;
//        case FL_Down:
//          if (pp.menu_number || !pp.menubar) {
//            forward(pp.menu_number);
//          } else if (pp.menu_number < pp.nummenus-1) {
//            forward(pp.menu_number+1);
//          }
//          return 1;
//        case FL_Right:
//        RIGHT:
//          if (pp.menubar && (pp.menu_number<=0 || (pp.menu_number == pp.nummenus-1)))
//            forward(0);
//          else if (pp.menu_number < pp.nummenus-1) forward(pp.menu_number+1);
//          return 1;
//        case FL_Left:
//          if (pp.menubar && pp.menu_number<=1) backward(0);
//          else if (pp.menu_number>0)
//            setitem(pp.menu_number-1, pp.p[pp.menu_number-1]->selected);
//          return 1;
//        case FL_Enter:
//        case FL_KP_Enter:
//        case ' ':
//          // if the current item is a submenu with no callback,
//          // simulate FL_Right to enter the submenu
//          if (   pp.current_item
//              && (!pp.menubar || pp.menu_number > 0)
//              && pp.current_item->activevisible()
//              && pp.current_item->submenu()
//              && !pp.current_item->callback_)
//          {
//            goto RIGHT;
//          }
//          // Ignore keypresses over inactive items, mark KEYBOARD event as used.
//          if (pp.current_item && !pp.current_item->activevisible())
//            return 1;
//          // Mark the menu 'done' which will trigger the callback
//          pp.state = DONE_STATE;
//          return 1;
        case FL_Escape:
          hide();
          Fl::grab(nullptr); // Or rather the next menu window up in a hierarchy
//          setitem(0, -1, 0);
//          pp.state = DONE_STATE;
          return 1;
      }
      break;
//    case FL_SHORTCUT:
//    {
//      for (int mymenu = pp.nummenus; mymenu--;) {
//        menuwindow &mw = *(pp.p[mymenu]);
//        int item; const Fl_Menu_Item* m = mw.menu->find_shortcut(&item);
//        if (m) {
//          setitem(m, mymenu, item);
//          if (!m->submenu()) pp.state = DONE_STATE;
//          return 1;
//        }
//      }
//    }
//      break;
//    case FL_MOVE: {
//      static int use_part1_extra = Fl::screen_driver()->need_menu_handle_part1_extra();
//      if (use_part1_extra && pp.state == DONE_STATE) {
//        return 1; // Fix for STR #2619
//      }
//    }
    case FL_RELEASE: {
      int ret = super::handle(e);
      fprintf(stderr, "FL_RELEASE\n");
      return ret; }
//      /* FALLTHROUGH */
//    case FL_ENTER:
    // TODO: MATT: calling FL_PUSH on Fl_Button will change Fl::grab() to the
    // button, and then on FL_RELEASE to nullptr. We have to decide if we want
    // to fix Fl_Button to restore the old Fl::grab(), or if we restore it
    // ourselves after FL_RELEASE, or if we hide the menu anyway.
    case FL_PUSH:
      fprintf(stderr, "FL_PUSH\n");
//    case FL_DRAG:
    {
      int mx = Fl::event_x_root();
      int my = Fl::event_y_root();
//      int item=0; int mymenu = pp.nummenus-1;
//      // Clicking or dragging outside menu cancels it...
//      if ((!pp.menubar || mymenu) && !pp.is_inside(mx, my)) {
      if ( (e==FL_PUSH) && (!Fl::event_inside(this)) ) {
        hide();
        Fl::grab(nullptr); // Or rather the next menu window up in a hierarchy
      }
//        setitem(0, -1, 0);
//        if (e==FL_PUSH)
//          pp.state = DONE_STATE;
//        return 1;
//      }
//      for (mymenu = pp.nummenus-1; ; mymenu--) {
//        item = pp.p[mymenu]->find_selected(mx, my);
//        if (item >= 0)
//          break;
//        if (mymenu <= 0) {
//          // buttons in menubars must be deselected if we move outside of them!
//          if (pp.menu_number==-1 && e==FL_PUSH) {
//            pp.state = DONE_STATE;
//            return 1;
//          }
//          if (pp.current_item && pp.menu_number==0 && !pp.current_item->submenu()) {
//            if (e==FL_PUSH) {
//              pp.state = DONE_STATE;
//              setitem(0, -1, 0);
//            }
//            return 1;
//          }
//          // all others can stay selected
//          return 0;
//        }
//      }
//      setitem(mymenu, item);
//      if (e == FL_PUSH) {
//        if (pp.current_item && pp.current_item->submenu() // this is a menu title
//            && item != pp.p[mymenu]->selected // and it is not already on
//            && !pp.current_item->callback_) // and it does not have a callback
//          pp.state = MENU_PUSH_STATE;
//        else
//          pp.state = PUSH_STATE;
//      }
      }
      break; // Nothing was handled. Let super do it.
//      return 1;
//    case FL_RELEASE:
//      // Mouse must either be held down/dragged some, or this must be
//      // the second click (not the one that popped up the menu):
//      if (   !Fl::event_is_click()
//          || pp.state == PUSH_STATE
//          || (pp.menubar && pp.current_item && !pp.current_item->submenu()) // button
//          ) {
//#if 0 // makes the check/radio items leave the menu up
//        const Fl_Menu_Item* m = pp.current_item;
//        if (m && button && (m->flags & (FL_MENU_TOGGLE|FL_MENU_RADIO))) {
//          ((Fl_Menu_*)button)->picked(m);
//          pp.p[pp.menu_number]->redraw();
//        } else
//#endif
//          // do nothing if they try to pick an inactive item, or a submenu with no callback
//          if (!pp.current_item || (pp.current_item->activevisible() &&
//                                   (!pp.current_item->submenu() || pp.current_item->callback_ || (pp.menubar && pp.menu_number <= 0))))
//            pp.state = DONE_STATE;
//      }
//      return 1;
  }
  return super::handle(e);
}
