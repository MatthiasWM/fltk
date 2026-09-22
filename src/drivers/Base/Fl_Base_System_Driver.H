//
// Definition of default (no-op) system driver.
//
// Copyright 2010-2026 by Bill Spitzak and others.
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

#ifndef FL_BASE_SYSTEM_DRIVER_H
#define FL_BASE_SYSTEM_DRIVER_H

#include "../../Fl_System_Driver.H"

#include <chrono>
#include <thread>

/**
 \brief The default System driver: does no work, but is safe to use standalone.

 The base class already provides safe default bodies for everything except
 wait(), whose default computes the time left to wait but does not actually
 block - reusing it unmodified would busy-spin an event loop at 100% CPU.
 This class sleeps for the remaining time instead.
*/
class Fl_Base_System_Driver : public Fl_System_Driver {
public:
  double wait(double time_to_wait) override {
    double t = Fl_System_Driver::wait(time_to_wait);
    if (t > 0) std::this_thread::sleep_for(std::chrono::duration<double>(t));
    return t;
  }
};

#endif // FL_BASE_SYSTEM_DRIVER_H
