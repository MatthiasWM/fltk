#
# Macro used by the CMake build system for the Fast Light Tool Kit (FLTK).
# Originally written by Matthias Melcher
#
# Copyright 2025 by Bill Spitzak and others.
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
# fl_include_all - include all .cmake files by the give name
#======================================================================
#
# Input:
#
#   CORE_ADDONS: a list of paths to core add-ons
#
#   INCLUDE_PATH: a path and file name of the file to include
#
# This macro loops through all paths in CORE_ADDONS. It creates the path
# to a .cmake file by appending INCLUDE_PATH to the add-on path. If this
# .camke file is found, it will be included into the CMake project.
#
# Included scripts can use FL_ADDON_BASE to find the absolute path to the
# add-on directory. Note that the path can contain spaces.
# The variable FL_ADDON_INCLUDE holds the absolute path of the included
# .cmake file.
#
#######################################################################

macro(fl_include_all CORE_ADDONS INCLUDE_PATH)

  #message(STATUS "  --- fl_include_all :: ${${CORE_ADDONS}} :: ${INCLUDE_PATH} ::")

  if(DEFINED ${CORE_ADDONS})
    foreach(ADDON_PATH ${${CORE_ADDONS}})
      get_filename_component(FL_ADDON_BASE
        ${ADDON_PATH} ABSOLUTE
        BASE_DIR ${PROJECT_SOURCE_DIR}
      )
      if (EXISTS "${FL_ADDON_BASE}")
        # CMake add_path is 3.20, we only require 3.15:
        string(CONCAT FL_ADDON_INCLUDE ${FL_ADDON_BASE} "/" ${INCLUDE_PATH} )
        #message(STATUS "      Checking for '${FL_ADDON_INCLUDE}'")
        if(EXISTS "${FL_ADDON_INCLUDE}")
          #message(STATUS "      Found! Including, base set at '${FL_ADDON_BASE}'")
          include("${FL_ADDON_INCLUDE}")
        else()
          #message(STATUS "      Not Found.")
        endif()
      else()
        message(WARNING "      Path not found: '${FL_ADDON_BASE}', derived from in FLTK_CORE_ADDONS.")
      endif()
    endforeach()
  else()
    #message(STATUS "      No addon paths defined.")
  endif()

  #message(STATUS "  --- fl_include_all done")

endmacro()

