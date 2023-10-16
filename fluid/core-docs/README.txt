README.txt (fluid/core-docs)
---------------------------
Documentation is available in HTML and PDF format built using 'doxygen'
since FLTK 1.3. The documentation must be generated in a separate step
or downloaded from FLTK's software download pages. The documentation can
also be accessed online.

To read the docs after downloading or generating them, open the files:

    documentation/fluid-core/html/index.html
    documentation/fluid-core.pdf

with your browser or PDF viewer, respectively.


Online Documentation (Latest Release):
--------------------------------------
A documentation version is available online at the FLTK web site,
along with the PDF version of the manual. The docs on the web site
are usually somewhat older (latest release). The active development
version (currently 1.4.0) is updated from time to time.

Use this URL to find the current online documentation:

    https://www.fltk.org/documentation.php


Online Documentation (Daily CI Build):
--------------------------------------
The documentation of the development branch 'master' is generated
daily in the CI (Continuous Integration) build on our GitLab mirror.
This is experimental and may be discontinued at any time.

Documentation can be found at these URL's:

    HTML: https://fltk.gitlab.io/fltk/fluid-core
    PDF:  https://fltk.gitlab.io/fltk/fluid-core.pdf


Generation of Documentation with autoconf (configure) + make:
-------------------------------------------------------------
Use "make fluid_core_html" in the documentation directory to generate the HTML
documentation, or "make fluid_core_pdf" to generate the PDF version.
"make fluid_core_docs" generates both versions in one step (as would do
"make clean fluid_core_html fluid_core_docs").


Generation of Documentation with CMake:
---------------------------------------
The following two options are predefined ON if you have the
required software packages (doxygen, LaTeX) installed. You can
always leave them ON because the documentation is not built
automatically (it is excluded from the default target "ALL").

  - OPTION_BUILD_FLUID_CORE_HTML_DOCS:BOOL=ON     target: fluid_core_html
  - OPTION_BUILD_FLUID_CORE_PDF_DOCS:BOOL=ON      target: fluid_core_pdf

The following two options default to OFF and can be switched ON.
They are only used when installing the software ('make install')
and the corresponding build options are ON.

  - OPTION_INSTALL_FLUID_CORE_HTML_DOCS:BOOL=OFF
  - OPTION_INSTALL_FLUID_CORE_PDF_DOCS:BOOL=OFF


Bugs and Feature Requests:
--------------------------
If you find any typos, things that are unclear, or would like to
contribute an example, section, or chapter to the FLTK manual, please
post a question in the fltk group fltk.general or post a bug report
or feature request. For more information see this page:

    https://www.fltk.org/bugs.php
