#
# CMakeLists.txt to build FLUID user docs for the FLTK project using CMake
#
# Copyright 2023 by Bill Spitzak and others.
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

set (FLUID_USER_DOCS)
set (GENERATE_FLUID_USER_DOCS FALSE)
set (GIT_REVISION "")
set (YEAR "")
set (CURRENT_DATE "")

if (OPTION_BUILD_FLUID_USER_HTML_DOCS OR OPTION_BUILD_FLUID_USER_PDF_DOCS)
  set (GENERATE_FLUID_USER_DOCS TRUE)
endif ()

#------------------------------------------------
# generate files used for both HTML and PDF docs
#------------------------------------------------

if (GENERATE_FLUID_USER_DOCS)

  # create required variables

  execute_process (COMMAND date "+%Y"
    OUTPUT_VARIABLE YEAR
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  # note: current locale is used for abbreviated month
  execute_process (COMMAND date "+%b %d, %Y"
    OUTPUT_VARIABLE CURRENT_DATE
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  # find git revision

  # FIXME: This must also work with tarballs where git is not available.
  #        For now we just ignore errors and set GIT_REVISION = "unkown".
  #        In the future tarball/zip generation should create a file
  #        that contains the git revision.

  execute_process (COMMAND
    git --git-dir=${FLTK_SOURCE_DIR}/.git rev-parse --short=10 HEAD
    OUTPUT_VARIABLE GIT_REVISION
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
  )

  # set to "'unkown'" if git is not available
  if (GIT_REVISION STREQUAL "")
    set (GIT_REVISION "'unkown'")
  endif()

  # Find "short" doxygen version if it was built from Git
  # Note: this is still needed in CMake 3.12.0 but later CMake versions
  #       (notably 3.25) remove the Git revision in 'DOXYGEN_VERSION'.
  # Todo: Find the "first good" CMake version and remove this redundant
  #       code once we require this as our minimal version and replace the
  #       variable DOXYGEN_VERSION_SHORT with DOXYGEN_VERSION below.

  if (DOXYGEN_FOUND)
    # strip trailing git revision if doxygen was built from source
    string (REGEX REPLACE " .*$" "" DOXYGEN_VERSION_SHORT ${DOXYGEN_VERSION})
  endif (DOXYGEN_FOUND)

  # configure copyright.dox (includes current year)
  configure_file (
    ${CMAKE_CURRENT_SOURCE_DIR}/copyright.dox.in
    ${CMAKE_CURRENT_BINARY_DIR}/copyright.dox
    @ONLY
  )

  # configure generated.dox (includes date and versions)
  configure_file (
    ${CMAKE_CURRENT_SOURCE_DIR}/generated.dox.in
    ${CMAKE_CURRENT_BINARY_DIR}/generated.dox
    @ONLY
  )

  if (0) # debug
    fl_debug_var (YEAR)
    fl_debug_var (CURRENT_DATE)
    fl_debug_var (GIT_REVISION)
    fl_debug_var (DOXYGEN_FOUND)
    fl_debug_var (DOXYGEN_EXECUTABLE)
    fl_debug_var (DOXYGEN_VERSION)
    fl_debug_var (DOXYGEN_VERSION_SHORT)
  endif ()

endif (GENERATE_FLUID_USER_DOCS)

#--------------------------
# build html documentation
#--------------------------

if (OPTION_BUILD_FLUID_USER_HTML_DOCS)

  list (APPEND DOCS fluid_user_html)

  # generate Doxygen file "Doxyfile"

  set (GENERATE_HTML  YES)
  set (GENERATE_LATEX NO)
  set (LATEX_HEADER   "")
  set (DOXYFILE       "Doxyfile")
  set (LOGFILE        "${CMAKE_CURRENT_BINARY_DIR}/${DOXYFILE}_error.log")

  # configure Doxygen input file for HTML docs (Doxyfile.in)

  configure_file (
    ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
    ${CMAKE_CURRENT_BINARY_DIR}/${DOXYFILE}.in
    @ONLY
  )

  # convert Doxyfile to used doxygen version

  add_custom_command (
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${DOXYFILE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/convert_doxyfile
            ${DOXYGEN_EXECUTABLE}
            ${DOXYFILE}.in
            ${DOXYFILE}
            ${LOGFILE}
    BYPRODUCTS ${LOGFILE}
    COMMENT "Converting ${DOXYFILE} to doxygen version ${DOXYGEN_VERSION_SHORT}" VERBATIM
  )

  # generate HTML documentation

  add_custom_target (fluid_user_html
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${DOXYFILE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating HTML documentation" VERBATIM
  )

endif (OPTION_BUILD_FLUID_USER_HTML_DOCS)

#--------------------------
# build pdf documentation
#--------------------------

if (OPTION_BUILD_FLUID_USER_PDF_DOCS)

  list (APPEND DOCS fluid_user_pdf)

  # generate Doxygen input file "Doxybook"

  set (GENERATE_HTML  NO)
  set (GENERATE_LATEX YES)
  set (LATEX_HEADER   "${CMAKE_CURRENT_BINARY_DIR}/fluid-user-book.tex")
  set (DOXYFILE       "Doxybook")
  set (LOGFILE        "${CMAKE_CURRENT_BINARY_DIR}/${DOXYFILE}_error.log")

  # configure Doxygen input file for PDF docs (Doxybook.in)

  configure_file (
    ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
    ${CMAKE_CURRENT_BINARY_DIR}/${DOXYFILE}.in
    @ONLY
  )

  # convert Doxybook to current doxygen version

  add_custom_command (
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${DOXYFILE}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/convert_doxyfile
            ${DOXYGEN_EXECUTABLE}
            ${DOXYFILE}.in
            ${DOXYFILE}
            ${LOGFILE}
    BYPRODUCTS ${LOGFILE}
    COMMENT "Converting ${DOXYFILE} to doxygen version ${DOXYGEN_VERSION_SHORT}" VERBATIM
  )

  # generate LaTeX title fluid-user-title.tex

  configure_file (
    ${CMAKE_CURRENT_SOURCE_DIR}/src/fluid-user-title.tex.in
    ${CMAKE_CURRENT_BINARY_DIR}/fluid-user-title.tex
    @ONLY
  )

  # generate fluid-user.pdf

  add_custom_command (
    OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/fluid-user.pdf
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/make_header
            ${DOXYGEN_EXECUTABLE}
            ${CMAKE_CURRENT_BINARY_DIR}/fluid-user-title.tex
            ${CMAKE_CURRENT_BINARY_DIR}/fluid-user-book.tex
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/make_pdf
    COMMAND cp -f latex/refman.pdf fluid-user.pdf
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${DOXYFILE}
            ${CMAKE_CURRENT_BINARY_DIR}/fluid-user-title.tex
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating PDF documentation" VERBATIM
  )

  # add target 'pdf'

  add_custom_target (fluid_user_pdf
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/fluid-user.pdf
  )

endif (OPTION_BUILD_FLUID_USER_PDF_DOCS)

#----------------------------------
# add target 'docs' for all docs
#----------------------------------

if (FLUID_USER_DOCS)

  add_custom_target (fluid_user_docs
    DEPENDS ${FLUID_USER_DOCS}
  )

endif (FLUID_USER_DOCS)

#----------------------------------
# install html + pdf documentation
#----------------------------------

if (OPTION_INSTALL_FLUID_USER_HTML_DOCS AND OPTION_BUILD_FLUID_USER_HTML_DOCS)

  install (DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html
    DESTINATION ${FLTK_DATADIR}/doc/fluid-user
  )

endif (OPTION_INSTALL_FLUID_USER_HTML_DOCS AND OPTION_BUILD_FLUID_USER_HTML_DOCS)

if (OPTION_INSTALL_FLUID_USER_PDF_DOCS AND OPTION_BUILD_FLUID_USER_PDF_DOCS)

  install (FILES ${CMAKE_CURRENT_BINARY_DIR}/fluid-user.pdf
    DESTINATION ${FLTK_DATADIR}/doc/
  )

endif (OPTION_INSTALL_FLUID_USER_PDF_DOCS AND OPTION_BUILD_FLUID_USER_PDF_DOCS)
