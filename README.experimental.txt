
Experimental Feature: Core Add-Ons
----------------------------------

The core add-on feature makes it possible to add widgets, tests, drivers, 
modules, and Fluid extensions from an external directory as if they were
part of the core FLTK library. An add-on can be seen as a step between
an external feature and full integration into the core library. The add-on can 
be tested and maintained externally for a while and then be integrated when
when matured enough.

Users integrate add-ons into the core library at configuration time by 
defining `FLTK_CORE_ADDONS`, for example:

```
cmake -B build -DFLTK_CORE_ADDONS="../Fl_Range_Slider/;../SDL_Driver"
```

Add-on have the same basic structure as the FLTK library. Sources are in the
`src/` directory, test in `tests/`, etc. . The add-on developer adds Cmake 
include files in '`Cmake/`.

To write a test add-on, a file `CMake/fl_addon_test.cmake` must be created. 
It is included by fltk when all other tests are added, so it can use the 
same commands that are available in the core library. For example:

```
fl_create_example(range_slider ${FL_ADDON_BASE}/test/range_slider.cxx fltk::fltk)
```

FL_ADDON_BASE is the absolute path to the direcotry given in FLTK_CORE_ADDONS.

\see https://github.com/MatthiasWM/fltk-addons-1.5 for an example add-on

```
git clone https://github.com/MatthiasWM/fltk.git
git clone https://github.com/MatthiasWM/fltk-addons-1.5.git
cd fltk
git checkout core_addons
cmake -B build -DFLTK_CORE_ADDONS="../fltk-addons-1.5/Fl_Range_Slider"
cmake --build build --target range_slider
build/bin/test/range_slider
```






Removed Experimental Platforms
------------------------------

Removed platforms and drivers include:

- Android
- Pico
- PicoAndroid
- PicoSDL

These platforms and their drivers were experimental and undocumented.

They have been removed in January 2022 for maintenance reasons as
discussed in fltk.coredev: "FLTK 1.4.0 release schedule", see:
https://groups.google.com/g/fltkcoredev/c/PDbHTRpXVh0/m/JqboexZ_AwAJ

More information, for instance where to find further development
(if any) will be added in this file.

If you want to take a look at the removed drivers you can checkout
the git tag "experimental-2022-01":

$ git clone https://github.com/fltk/fltk.git fltk-experimental
$ cd fltk-experimental
$ git checkout experimental-2022-01

You can also browse the files on GitHub:

https://github.com/fltk/fltk/tree/b275ff07158e80d1744ddb2f6c51094a87cf079a
