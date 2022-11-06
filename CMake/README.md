
# the FLTK CMake build system

This document is meant to give an overview over the CMake build process.

## CMake top level calling order

```
CMakeLists.txt
 |
 +- CMake/fl_debug_var.cmake
 |   +- macro fl_debug_var name
 +- CMake/fl_debug_pkg.cmake
 |   +- macro fl_debug_pkg PREFIX NAME
 +- CMake/fl_add_library.cmake
 |   +- macro FL_ADD_LIBRARY LIBNAME LIBTYPE LIBFILES
 +- CMake/compatibility.cmake
 |   +- func fl_target_link_directories TARGET SCOPE DIRS
 +- CMake/setup.cmake
 |   +- # Basic initialisation and some configuration tests
 +- CMake/resources.cmake
 |   +- macro fl_find_header VAR HEADER
 +- CMake/options.cmake
 |   +- zlib/CMakeLists.txt ...
 |   +- jpeg/CMakeLists.txt ...
 |   +- png/CMakeLists.txt ...
 |   +- zlib/CMakeLists.txt ...
 |   +- documentation/CMakeLists.txt
 |   |   +- copyright.dox.in -> copyright.dox
 |   |   +- generated.dox.in -> generated.dox
 |   |   +- Doxyfile.in -> $CBINDIR/Doxyfile.in
 |   |   +- Doxyfile.in -> $CBINDIR/Doxybook.in
 |   |   +- src/fltk-title.tex.in -> fltk-title.tex
 |   +- cairo/CMakeLists.txt
 +- src/CMakeLists.txt
 |   +- # Initiate libraries
 +- fluid/CMakeLists.txt
 |   +- # Initiate fluid app
 +- CMake/variables.cmake
 |   +- # Construct variable for compilation and linking
 +- CMake/export.cmake
 |   +- CMake/FLTK-Functions.cmake -> $CBINDIR/FLTK-Functions.cmake
 |   +- CMake/FLTKConfig.cmake.in -> $CBINDIR/FLTKConfig.cmake
 |   |   +- FLTK-Targets.cmake
 |   +- CMake/UseFLTK.cmake.in -> $CBINDIR/UseFLTK.cmake (Deprecated)
 |   +- fltk-config.in -> $CBINDIR/fltk-config
 |   |   +- # this script shows the correct flags to compile and link
 |   +- configh.cmake.in -> $CBINDIR/config.h
 |   +- CMake/install-symlinks.cmake.in -> $CBINDIR/install-symlinks.cmake
 +- fl_config.cmake.in -> FL/fl_config.h
 |   +- # Define some options for use in source code
 +- test/CMakeLists.txt
 |   +- CMake/FLTK-Functions.cmake
 |   |   +- function FLTK_RUN_FLUID TARGET SOURCES
 |   |   +- function FLTK_SET_BUNDLE_ICON TARGET ICON_PATH
 |   +- CMake/fl_create_example.cmake
 |       +- macro (CREATE_EXAMPLE NAME SOURCES LIBRARIES)
 +- examples/CMakeLists.txt
 |   +- CMake/fl_create_example.cmake (as above)
 +- CMake/install.cmake
     +- CMake/cmake_uninstall.cmake.in -> $CBINDIR/cmake_uninstall.cmake
     +- install-symlinks.cmake
     +- FLTK-Targets.cmake
     +- CMake/FLTKConfig.cmake.in -> $CBINDIR/etc/FLTKConfig.cmake
     +- etc/FLTKConfig.cmake
     +- CMake/FLTK-Functions.cmake
     +- CMake/UseFLTK.cmake.in -> $CBINDIR/etc/UseFLTK.cmake
     +- etc/UseFLTK.cmake
     +- fltk-config.in -> $CBINDIR/bin/fltk-config
     +- bin/fltk-config
     +- macro INSTALL_MAN FILE LEVEL -> install man pages 
 ```
 
 
