//
// Code editor widget for the Fast Light Tool Kit (FLTK).
// Syntax highlighting rewritten by erco@seriss.com 09/15/20.
//
// Copyright 1998-2021 by Bill Spitzak and others.
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

#ifndef CodeEditor_h
#define CodeEditor_h

//
// Include necessary headers...
//

#include "StyleParse.h"

#include <FL/Fl.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Text_Editor.H>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ---- CodeEditor declaration

class CodeEditor : public Fl_Text_Editor {
  friend class StyleParse;

  static Fl_Text_Display::Style_Table_Entry styletable[];
  static void style_parse(const char *tbuff, char *sbuff, int len, char style);
  static void style_unfinished_cb(int, void*);
  static void style_update(int pos, int nInserted, int nDeleted,
                           int /*nRestyled*/, const char * /*deletedText*/,
                           void *cbArg);
  static int auto_indent(int, CodeEditor* e);

public:
  CodeEditor(int X, int Y, int W, int H, const char *L=0);
  ~CodeEditor();
  int top_line() { return get_absolute_top_line_number(); }
  void textsize(Fl_Fontsize s);
  int scroll_row() { return mTopLineNum; }
  int scroll_col() { return mHorizOffset; }
  void draw();
};

// ---- CodeRangeEditor declaration

class CodeRangeEditor;
class Fl_Type;

typedef void (*CodeRangeEditorCallback)(class CodeRangeEditor*, class Fl_Type*);

/*
 This editor allows users to edit a renge of text. No text can be changed
 outside the give range. If users leave focus or leave the editing range,
 a callback will occur, returning the modified code range.

 Fluid uses this editor
 */
class CodeRangeEditor : public CodeEditor {
  int event_position_;
  int event_button_;
  int update_suspended_;
  CodeRangeEditorCallback focus_lost_cb_;
  Fl_Type *focus_lost_widget_;
  int editable_start_, editable_end_;
public:
  CodeRangeEditor(int X, int Y, int W, int H, const char *L=0);
  ~CodeRangeEditor();
  int event_position() const { return event_position_; }
  int event_button() const { return event_button_; }
  char *event_text() const;
  void make_editable(int pos_a, int pos_b, CodeRangeEditorCallback cb, Fl_Type *w);
  int is_editable() const { return (editable_start_!=editable_end_); }
  void end_editable(char call_the_callback=1);
  void suspend_update() { update_suspended_++; }
  void restore_update() { update_suspended_--; }
  int update_suspended() { return (update_suspended_!=0); }
protected:
  int handle(int event);
  static void edit_range_update(int pos, int nInserted, int nDeleted,
                           int /*nRestyled*/, const char * /*deletedText*/,
                           void *cbArg);
};

// ---- CodeViewer declaration

class CodeViewer : public CodeEditor {
public:
  CodeViewer(int X, int Y, int W, int H, const char *L=0);

protected:
  int handle(int ev) { return Fl_Text_Display::handle(ev); }
};

#endif // !CodeEditor_h
