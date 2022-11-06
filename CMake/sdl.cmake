#
# SDL support to build the FLTK project using CMake (www.cmake.org)
#
# Copyright 1998-2022 by Bill Spitzak and others.
#
# This library is free software. Distribution and use rights are outlined in
# the file "COPYING" which should have been included with this file.  If this
# file is missing or damaged, see the license at:
#
#     https://www.fltk.org/COPYING.php
#
# Please see the following page on how to report bugs and issues:
#
#     https://www.fltk.org/bugs.php
#

#######################################################################
# Check the configuration matrix and let the user know if SDL can be
# supported in this configuration.

if (APPLE)
  if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    # git clone ...
    # cmake -S . -B build
    # cmake --build build --target install
    message (WARNING "FLTK_USE_SDL on macOS targeting Darwin is under development" )
    find_package(SDL2 REQUIRED)
  elseif (CMAKE_SYSTEM_NAME STREQUAL "iOS")
    message (FATAL_ERROR "FLTK_USE_SDL on macOS targeting iOS is not supported yet" )
  elseif (CMAKE_SYSTEM_NAME STREQUAL "Android")
    message (FATAL_ERROR "FLTK_USE_SDL on macOS targeting iOS is not supported yet" )
  else ()
    message (FATAL_ERROR "FLTK_USE_SDL on macOS targeting ${CMAKE_SYSTEM_NAME} is not supported" )
  endif ()
elseif (WIN32)
  message (FATAL_ERROR "FLTK_USE_SDL on MSWindows targeting ${CMAKE_SYSTEM_NAME} is not supported yet" )
elseif (UNIX)
  message (FATAL_ERROR "FLTK_USE_SDL on Unix targeting ${CMAKE_SYSTEM_NAME} is not supported yet" )
else ()
  message (FATAL_ERROR "FLTK_USE_SDL on on this platfrom is not supported" )
endif ()
