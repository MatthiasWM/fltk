
This README fiel must eventually be incorporated into the new documentation
system that will reduce the root directory to a single README.txt .

This document describes the FLTK build matrixx and in particular the SDL
backend implemenatation, that will allow FLTK apps to run on Android,
iOS, RasPi, and more.

Backend matrix:

Target OS > |  Winddows  |   Linux    |   macOS    |    iOS     |   Android
v Host OS v |            |            |            |            |
------------+------------+------------+------------+------------+------------
    Windows | WinAPI/GDI |            |            | Posix/SDL* | Posix/SDL*
            | Posix/SDL* |            |            |            |
------------+------------+------------+------------+------------+------------
      Linux |            | Posix/X11  |            | Posix/SDL* | Posix/SDL*
            |            | P./Wayland |            |            |
            |            | Posix/SDL* |            |            |
------------+------------+------------+------------+------------+------------
      macOS |            |            | D./Cocoa   | Posix/SDL* | Posix/SDL*
            |            |            | Darwin/X11 |            |
            |            |            | D./SDL**   |            |

* = Not Yet Implemented
** = work in progress

SDL is a "C" library that runs on very many platforms and provides a good
backend for most of FLTK's features. Original SDL code (mostly games and
emulators) can coexist with FLTK, providing a GUI that works and looks the
same on all platforms.

FLTK assumes that SDL can open one single window, which is common on mobile
devices. FLTK emulates multiple windows inside a single SLD screen by
implementin a minimal window manager, including window decoartion, leyering,
and clipping.

The current SDL_Surface is available via fl_gc. All SDL events are available
via Fl::add_system_handler(), so FLTK can be added on top of existing SDL
applications.

On desktop machines, FLTK currently uses also only a single window with its
internal window manager to allow for easy mobile development without an
emulator. It may make sense to support SDL multi window mode on desktops at a
later point.

A central part of the FLTK implementation is the ability to use CMake alone
for as many host/target/backend combinations as possible.

Install SDL for the target platform first.

To enable SDL, add -DFLTK_USE_SDL=on during CMake configuration.

To anbale iOS builds, add -DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_SYSROOT=iphonesimulator
or -DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_SYSROOT=iphoneos -DCMAKE_OSX_ARCHITECTURES="arm64;armv7s"


iOS build:
SDL:
cmake -S . -B build/iOS -DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_SYSROOT=iphonesimulator -DCMAKE_OSX_ARCHITECTURES=arm64
cmake --build build/iOS
sudo cmake --install build/iOS --prefix /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator16.0.sdk/usr/
FLTK:
cmake -S . -B build/iOS -G Xcode -DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_SYSROOT=iphonesimulator -DCMAKE_OSX_ARCHITECTURES=arm64 -DFLTK_USE_SDL=on
open build/iOS/FLTK.xcodeproj
generates and runs on simulator:
build/iOS/bin/test/Debug/adjuster.app

macOS build:
SDL:
cmake -S . -B build/macOS
cmake --build build/macOS --config Debug
sudo cmake --install build/macOS --config Debug
FLTK:
cmake -S . -B build/macOS -G Xcode -DFLTK_USE_SDL=on
open build/macOS/FLTK.xcodeproj
generates and runs on simulator:
build/macOS/bin/test/Debug/adjuster.app
