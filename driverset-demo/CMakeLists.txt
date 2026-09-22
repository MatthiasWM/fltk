#
# Standalone CMake project for the external driver-set demo.
#
# Deliberately NOT part of the main FLTK CMake project (no add_subdirectory()
# anywhere references this directory) - it consumes an already-built FLTK
# the way a real out-of-tree project would, via FLTK's exported CMake
# package config.
#
# Usage:
#   cmake -B build -S . -GNinja \
#     -DFLTK_DIR=<path to an FLTK build tree, e.g. .../build/ninja> \
#     -DFLTK_SOURCE_DIR=<path to the FLTK source checkout>
#   cmake --build build
#
# FLTK_DIR is the standard CMake find_package(CONFIG) hint variable.
# FLTK_SOURCE_DIR is specific to this demo: it's needed only to reach
# FLTK's private driver headers under src/ (see Fl_Demo_Driver_Set.cxx).
#

cmake_minimum_required(VERSION 3.15)

# find_package(FLTK) marks fltk::fltk's include dirs as SYSTEM (-isystem
# flags). If a stale FLTK happens to be installed system-wide (e.g. under
# /usr/local/include), mixing explicit -I and -isystem flags in the same
# compile can let that stale copy win over this source tree's own headers
# (observed with Apple clang). CMAKE_NO_SYSTEM_FROM_IMPORTED makes CMake
# emit imported targets' include dirs as plain -I instead, avoiding the
# ambiguity entirely. Must be set before find_package(FLTK).
set(CMAKE_NO_SYSTEM_FROM_IMPORTED ON)

project(fltk_demo_driverset CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT FLTK_SOURCE_DIR)
  message(FATAL_ERROR "Pass -DFLTK_SOURCE_DIR=<path to the fltk source checkout> (needed for private driver headers under src/)")
endif()

find_package(FLTK CONFIG REQUIRED)

# The external driver-set library: Base drivers + the real System driver.
#
# This links FLTK's *headers* (via fltk::fltk's INTERFACE_INCLUDE_DIRECTORIES)
# but deliberately does NOT link FLTK's static library into this shared
# library. FLTK here is built as libfltk.a (static); if this .dylib linked
# it too, every symbol from libfltk.a - including the Objective-C classes
# from Fl_cocoa.mm - would be duplicated between this .dylib and whatever
# host executable also links libfltk.a, which the Objective-C runtime
# rightly complains about at load time (observed: "Class FLWindow is
# implemented in both ..."). A real plugin should resolve FLTK's symbols
# against the host process that loads it via Fl_Plugin_Manager::load()
# (dlopen), not carry its own copy. -undefined dynamic_lookup is the
# macOS way to say "leave these unresolved until dlopen time, then look
# them up in the loading process." (The Linux equivalent needs no
# extra flag: a .so simply leaves symbols unresolved until load, as long
# as the host executable was linked normally.)
add_library(fltk_demo_driverset SHARED Fl_Demo_Driver_Set.cxx)
target_include_directories(fltk_demo_driverset PRIVATE ${FLTK_SOURCE_DIR}/src)
target_include_directories(fltk_demo_driverset PRIVATE ${FLTK_INCLUDE_DIRS})
set_target_properties(fltk_demo_driverset PROPERTIES PREFIX "lib")
if(APPLE)
  target_link_options(fltk_demo_driverset PRIVATE -undefined dynamic_lookup)
endif()

# A small host app that loads the driver set above at runtime and proves
# it's active. Needs the private driver headers too, since proving the
# real System driver is active means calling a method on it directly.
add_executable(host_demo host_demo.cxx)
target_include_directories(host_demo PRIVATE ${FLTK_SOURCE_DIR}/src)
target_link_libraries(host_demo PRIVATE fltk::fltk)
