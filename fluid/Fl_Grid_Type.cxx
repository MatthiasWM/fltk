//
// Fl_Grid object code for the Fast Light Tool Kit (FLTK).
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

#include "Fl_Grid_Type.h"

#include "fluid.h"
#include "file.h"
#include "code.h"
#include "widget_browser.h"
#include "undo.h"
#include "Fd_Snap_Action.h"
#include "custom_widgets.h"

#include <FL/Fl_Grid.H>
#include <FL/Fl_Value_Input.H>
#include "../src/flstring.h"

#include <stdio.h>
#include <stdlib.h>

// ---- Fl_Grid_Type --------------------------------------------------- MARK: -

const char grid_type_name[] = "Fl_Grid";

Fl_Grid_Type Fl_Grid_type;      // the "factory"

Fl_Grid_Type::Fl_Grid_Type() {
}

#include <FL/Fl_Button.H>

Fl_Widget *Fl_Grid_Type::widget(int X,int Y,int W,int H) {
  Fl_Grid *g = new Fl_Grid(X,Y,W,H);
  g->layout(3, 3);
//  Fl_Button *b0 = new Fl_Button(0, 0, 100, 25, "Hello");
//  g->widget(b0, 0, 0);
//  Fl_Button *b1 = new Fl_Button(0, 0, 100, 25, "Huhu");
//  g->widget(b1, 0, 1);
//  Fl_Button *b2 = new Fl_Button(0, 0, 100, 25, "Hey!");
//  g->widget(b2, 1, 2);
  Fl_Group::current(0);
  return g;
}

Fl_Widget *Fl_Grid_Type::enter_live_mode(int) {
  Fl_Grid *grd = new Fl_Grid(o->x(), o->y(), o->w(), o->h());
  propagate_live_mode(grd);
  Fl_Grid *d = grd, *s =(Fl_Grid*)o;
//  int nc = s->children(), nd = d->children();
//  if (nc>nd) nc = nd;
//  for (int i=0; i<nc; i++) {
//    if (s->fixed(s->child(i))) {
//      Fl_Widget *dc = d->child(i);
//      d->fixed(d->child(i), s->horizontal() ? dc->w() : dc->h());
//    }
//  }
  return grd;
}

void Fl_Grid_Type::copy_properties()
{
  Fl_Group_Type::copy_properties();
  Fl_Grid *d = (Fl_Grid*)live_widget, *s =(Fl_Grid*)o;
  int lm, tm, rm, bm;
  s->margin(&lm, &tm, &rm, &bm);
  d->margin(lm, tm, rm, bm);
  int rg, cg;
  s->gap(&rg, &cg);
  d->gap(rg, cg);
}

void Fl_Grid_Type::write_properties(Fd_Project_Writer &f)
{
  Fl_Group_Type::write_properties(f);
  Fl_Grid* grid = (Fl_Grid*)o;
  int lm, tm, rm, bm;
  grid->margin(&lm, &tm, &rm, &bm);
  if (lm!=0 || tm!=0 || rm!=0 || bm!=0)
    f.write_string("margin {%d %d %d %d}", lm, tm, rm, bm);
  int rg, cg;
  grid->gap(&rg, &cg);
  if (rg!=0 || cg!=0)
    f.write_string("gap {%d %d}", rg, cg);
//  int nSet = 0;
//  for (int i=0; i<flex->children(); i++)
//    if (flex->fixed(flex->child(i)))
//      nSet++;
//  if (nSet) {
//    f.write_string("fixed_size_tuples {%d", nSet);
//    for (int i=0; i<flex->children(); i++) {
//      Fl_Widget *ci = flex->child(i);
//      if (flex->fixed(ci))
//        f.write_string(" %d %d", i, flex->horizontal() ? ci->w() : ci->h());
//    }
//    f.write_string("}");
//  }
}

void Fl_Grid_Type::read_property(Fd_Project_Reader &f, const char *c)
{
  Fl_Grid* grid = (Fl_Grid*)o;
  if (!strcmp(c,"margin")) {
    int lm, tm, rm, bm;
    if (sscanf(f.read_word(),"%d %d %d %d", &lm, &tm, &rm, &bm) == 4)
      grid->margin(lm, tm, rm, bm);
  } else if (!strcmp(c,"gap")) {
    int rg, cg;
    if (sscanf(f.read_word(),"%d %d", &rg, &cg))
      grid->gap(rg, cg);
//  } else if (!strcmp(c,"fixed_size_tuples")) {
//    f.read_word(1); // must be '{'
//    const char *nStr = f.read_word(1); // number of indices in table
//    fixedSizeTupleSize = atoi(nStr);
//    fixedSizeTuple = new int[fixedSizeTupleSize*2];
//    for (int i=0; i<fixedSizeTupleSize; i++) {
//      const char *ix = f.read_word(1); // child at that index is fixed in size
//      fixedSizeTuple[i*2] = atoi(ix);
//      const char *size = f.read_word(1); // fixed size of that child
//      fixedSizeTuple[i*2+1] = atoi(size);
//    }
//    f.read_word(1); // must be '}'
  } else {
    Fl_Group_Type::read_property(f, c);
  }
}

//void Fl_Grid_Type::postprocess_read()
//{
//  if (fixedSizeTupleSize==0) return;
//  Fl_Flex* flex = (Fl_Flex*)o;
//  for (int i=0; i<fixedSizeTupleSize; i++) {
//    int ix = fixedSizeTuple[2*i];
//    int size = fixedSizeTuple[2*i+1];
//    if (ix>=0 && ix<flex->children()) {
//      Fl_Widget *ci = flex->child(ix);
//      flex->fixed(ci, size);
//    }
//  }
//  fixedSizeTupleSize = 0;
//  delete[] fixedSizeTuple;
//  fixedSizeTuple = NULL;
//  flex->layout();
//  suspend_auto_layout = 0;
//}

void Fl_Grid_Type::write_code2(Fd_Code_Writer& f) {
  const char *var = name() ? name() : "o";
  Fl_Grid* grid = (Fl_Grid*)o;
  int lm, tm, rm, bm;
  grid->margin(&lm, &tm, &rm, &bm);
  if (lm!=0 || tm!=0 || rm!=0 || bm!=0)
    f.write_c("%s%s->margin(%d, %d, %d, %d);\n", f.indent(), var, lm, tm, rm, bm);
  int rg, cg;
  grid->gap(&rg, &cg);
  if (rg!=0 || cg!=0)
    f.write_c("%s%s->gap(%d, %d);\n", f.indent(), var, rg, cg);
//  for (int i=0; i<flex->children(); ++i) {
//    Fl_Widget *ci = flex->child(i);
//    if (flex->fixed(ci))
//      f.write_c("%s%s->fixed(%s->child(%d), %d);\n", f.indent(), var, var, i,
//                flex->horizontal() ? ci->w() : ci->h());
//  }
  Fl_Group_Type::write_code2(f);
}

void Fl_Grid_Type::add_child(Fl_Type* a, Fl_Type* b) {
  Fl_Group_Type::add_child(a, b);
  Fl_Grid* grid = (Fl_Grid*)o;
  int i, j;
  for (i=0; i<grid->rows(); ++i) {
    for (j=0; j<grid->cols(); ++j) {
      if (grid->cell(i, j)==NULL) goto done;
    }
  }
done:
  if (i==grid->rows()) {
    grid->layout(grid->rows()+1, grid->cols());
    j = 0;
  }
  if (a->is_true_widget()) {
    Fl_Widget_Type *wt = (Fl_Widget_Type*)a;
    grid->widget(wt->o, i, j);
    grid->need_layout(1);
    grid->redraw();
  }
}

//void Fl_Grid_Type::move_child(Fl_Type* a, Fl_Type* b) {
//  Fl_Group_Type::move_child(a, b);
//  if (!suspend_auto_layout)
//    ((Fl_Flex*)o)->layout();
//}
//
//void Fl_Grid_Type::remove_child(Fl_Type* a) {
//  if (a->is_widget())
//    ((Fl_Flex*)o)->fixed(((Fl_Widget_Type*)a)->o, 0);
//  Fl_Group_Type::remove_child(a);
//  ((Fl_Flex*)o)->layout();
//}

void grid_cb(Fl_Value_Input* i, void* v, int what) {
  if (v == LOAD) {
    if (current_widget->is_a(Fl_Type::ID_Grid)) {
      int v; 
      Fl_Grid *g = ((Fl_Grid*)current_widget->o);
      switch (what) {
        case 0: g->margin(&v, NULL, NULL, NULL); break;
        case 1: g->margin(NULL, &v, NULL, NULL); break;
        case 2: g->margin(NULL, NULL, &v, NULL); break;
        case 3: g->margin(NULL, NULL, NULL, &v); break;
        case 4: g->gap(&v, NULL); break;
        case 5: g->gap(NULL, &v); break;
      }
      i->value(v);
    }
  } else {
    int mod = 0;
    int v = (int)i->value();
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_a(Fl_Type::ID_Grid)) {
        Fl_Grid *g = ((Fl_Grid*)(((Fl_Widget_Type*)o)->o));
        switch (what) {
          case 0: g->margin(v, -1, -1, -1); break;
          case 1: g->margin(-1, v, -1, -1); break;
          case 2: g->margin(-1, -1, v, -1); break;
          case 3: g->margin(-1, -1, -1, v); break;
          case 4: g->gap(v, -1); break;
          case 5: g->gap(-1, v); break;
        }
        g->need_layout(true);
        g->redraw();
        mod = 1;
      }
    }
    if (mod) set_modflag(1);
  }
}
void grid_margin_left_cb(Fl_Value_Input* i, void* v) {
  grid_cb(i, v, 0);
}
void grid_margin_top_cb(Fl_Value_Input* i, void* v) {
  grid_cb(i, v, 1);
}
void grid_margin_right_cb(Fl_Value_Input* i, void* v) {
  grid_cb(i, v, 2);
}
void grid_margin_bottom_cb(Fl_Value_Input* i, void* v) {
  grid_cb(i, v, 3);
}
void grid_row_gap_cb(Fl_Value_Input* i, void* v) {
  grid_cb(i, v, 4);
}
void grid_col_gap_cb(Fl_Value_Input* i, void* v) {
  grid_cb(i, v, 5);
}

void grid_cb(Fluid_Coord_Input* i, void* v, int what) {
  if (v == LOAD) {
    if (current_widget->is_a(Fl_Type::ID_Grid)) {
      int v;
      Fl_Grid *g = ((Fl_Grid*)current_widget->o);
      switch (what) {
        case 6: v = g->rows(); break;
        case 7: v = g->cols(); break;
      }
      i->value(v);
    }
  } else {
    int mod = 0;
    int old_v = 0, v = (int)i->value();
    for (Fl_Type *o = Fl_Type::first; o; o = o->next) {
      if (o->selected && o->is_a(Fl_Type::ID_Grid)) {
        Fl_Grid *g = ((Fl_Grid*)(((Fl_Widget_Type*)o)->o));
        switch (what) {
          case 6: old_v = g->rows(); break;
          case 7: old_v = g->cols(); break;
        }
        if (old_v != v) {
          switch (what) {
            case 6: g->layout(v, g->cols()); break;
            case 7: g->layout(g->rows(), v); break;
          }
          g->need_layout(true);
          g->redraw();
          mod = 1;
        }
      }
    }
    if (mod) set_modflag(1);
  }
}
void grid_rows_cb(Fluid_Coord_Input* i, void* v) {
  grid_cb(i, v, 6);
}
void grid_cols_cb(Fluid_Coord_Input* i, void* v) {
  grid_cb(i, v, 7);
}

void grid_child_cb(Fluid_Coord_Input* i, void* v, int what) {
  if (   !current_widget
      || !current_widget->parent
      || !current_widget->parent->is_a(Fl_Type::ID_Grid))
  {
    return;
  }
  Fl_Grid *g = ((Fl_Grid*)((Fl_Widget_Type*)current_widget->parent)->o);
  if (v == LOAD) {
    int v = -1;
    Fl_Grid::Cell *cell = g->cell(current_widget->o);
    if (cell) {
      switch (what) {
        case 8: v = cell->row(); break;
        case 9: v = cell->col(); break;
      }
    }
    i->value(v);
  } else {
    int mod = 0;
    int v2 = 0, old_v = -1, v = (int)i->value();
    Fl_Grid::Cell *cell = g->cell(current_widget->o);
    if (cell) {
      switch (what) {
        case 8: old_v = cell->row(); v2 = cell->col(); break;
        case 9: old_v = cell->col(); v2 = cell->row(); break;
      }
    }
    if (old_v != v) {
      switch (what) {
        case 8: g->widget(current_widget->o, v, v2); break;
        case 9: g->widget(current_widget->o, v2, v); break;
      }
      g->need_layout(true);
      g->redraw();
      mod = 1;
      if (mod) set_modflag(1);
    }
  }
}
void grid_set_row_cb(Fluid_Coord_Input* i, void* v) {
  grid_child_cb(i, v, 8);
}
void grid_set_col_cb(Fluid_Coord_Input* i, void* v) {
  grid_child_cb(i, v, 9);
}
void grid_set_colspan_cb(Fluid_Coord_Input* i, void* v) {
  grid_child_cb(i, v, 10);
}
void grid_set_rowspan_cb(Fluid_Coord_Input* i, void* v) {
  grid_child_cb(i, v, 11);
}
void grid_align_cb(Fl_Choice* i, void* v) {
}
