#
# Makefile for the Fast Light Tool Kit (FLTK) FLUID user documentation.
#
# Copyright 1998-2023 by Bill Spitzak and others.
#
# This library is free software. Distribution and use rights are outlined in
# the file "COPYING" which should have been included with this file.  If this
# file is missing or damaged, see the license at:
#
#     https://www.fltk.org/COPYING.php
#
# Please see the following page on how to report bugs and issues:
#
#      https://www.fltk.org/bugs.php
#

# Get configuration stuff...
include ../../makeinclude

# make sure that all docs are (re-)created independent of missing deps
.PHONY: fluid-user-pdf fluid-user-html refman.pdf

SRC_DOCDIR      = 	./src

# These are the HTML "source" files...
HTMLFILES	= 	 \
			 $(SRC_DOCDIR)/index.dox \
			 $(SRC_DOCDIR)/fluid.dox

all:

# Use `make docs' to create all docs for distribution files.
# You need installed versions of Doxygen and LaTeX for this.

docs:	all fluid-user-html fluid-user-pdf

# Synonyms for docs:

alldocs: fluid-user-docs
dist:	fluid-user-docs

clean:
	$(RM) Doxyfile Doxybook
	$(RM) copyright.dox generated.dox
	$(RM) fluid-user.pdf refman.pdf src/fluid-user-title.tex src/fluid-user-book.tex.in src/fluid-user-book.tex
	$(RMDIR) html latex
	$(RM) *~ *.bck *.bak *.log
	$(RM) $(MANPAGES) $(SRC_DOCDIR)/*.0

depend:

install:
	echo "Installing documentation files in $(DESTDIR)$(docdir) ..."
	-$(INSTALL_DIR) $(DESTDIR)$(docdir)
	if test -f html/index.html ; then \
	  for file in html/* ; do \
		$(INSTALL_DATA) $$file $(DESTDIR)$(docdir); \
	  done \
	fi
	if test -f fltk.pdf ; then \
	  echo "Installing fltk.pdf in $(DESTDIR)$(docdir) ..."; \
	  $(INSTALL_DATA) fluid-user.pdf $(DESTDIR)$(docdir); \
	fi

uninstall:
	$(RMDIR) $(DESTDIR)$(docdir)

Doxyfile: Doxyfile.in generated.dox copyright.dox
	echo "Generating Doxyfile ..."
	./convert_doxyfile $(DOXYDOC) $< $@ Doxyfile_error.log
	sed -e's,@FLTK_VERSION@,$(FLTK_VERSION),' \
	    -e's,@GENERATE_HTML@,YES,' \
	    -e's,@GENERATE_LATEX@,NO,' \
	    -e's, @LATEX_HEADER@,,' \
	    -e's,@CMAKE_CURRENT_SOURCE_DIR@,.,' \
	    -e's,@CMAKE_CURRENT_BINARY_DIR@,,' \
	    -e's,@FLTK_SOURCE_DIR@,..,' \
	    -i.bak $@

Doxybook: Doxyfile.in generated.dox copyright.dox
	echo "Generating Doxybook ..."
	./convert_doxyfile $(DOXYDOC) $< $@ Doxybook_error.log
	$(DOXYDOC) -u -s - < $< 2>Doxybook_error.log | \
	sed -e's,@FLTK_VERSION@,$(FLTK_VERSION),' \
	    -e's,@GENERATE_HTML@,NO,' \
	    -e's,@GENERATE_LATEX@,YES,' \
	    -e's,@LATEX_HEADER@,src/fltk-book.tex,' \
	    -e's,@CMAKE_CURRENT_SOURCE_DIR@,.,' \
	    -e's,@CMAKE_CURRENT_BINARY_DIR@,,' \
	    -e's,@FLTK_SOURCE_DIR@,..,' \
	    -i.bak $@

# The HTML files are generated using doxygen, and this needs
# an installed doxygen version and may take some time, so this target
# is not made by default.
# Use `make html' or `make docs' to create the html docs.

fluid-user-html:	$(HTMLFILES) Doxyfile
	echo "Generating HTML FLUID user documentation..."
	-$(RMDIR) html
	-$(INSTALL_DIR) html
	-$(DOXYDOC)
	if test "x$(DOXYDOC)" = "x" ; then \
	  echo "Sorry - doxygen not found. Please install doxygen and run configure."; \
	fi

# The PDF documentation (fluid-user.pdf) is generated using doxygen and LaTeX, and
# this needs installed Doxygen and LaTeX programs and may take some time, so
# this target is not made by default.
# Use `make pdf' or `make docs' to create the PDF docs.

fluid-user-pdf:	refman.pdf
	cp -f latex/refman.pdf fluid-user.pdf

refman.pdf: $(HTMLFILES) Doxybook src/fluid-user-book.tex
	-$(RMDIR) latex
	echo "Generating PDF documentation ..."
	$(DOXYDOC) Doxybook
	./make_pdf

src/fluid-user-title.tex: src/fluid-user-title.tex.in
	echo "Generating $@ ..."
	GIT_REVISION=`git rev-parse --short=10 HEAD`; \
	sed -e"s/@GIT_REVISION@/$$GIT_REVISION/g" \
	    < $< > $@

src/fluid-user-book.tex.in: src/fluid-user-title.tex
	echo "Generating $@ ..."
	./make_header $(DOXYDOC) $< $@

src/fluid-user-book.tex: src/fluid-user-book.tex.in
	echo "Generating $@ ..."
	DOXY_VERSION=`$(DOXYDOC) --version|cut -f1 -d' '`; \
	YEAR=`date +%Y`; \
	sed -e"s/@YEAR@/$$YEAR/g" \
	    -e"s/@FLTK_VERSION@/$(FLTK_VERSION)/g" \
	    -e"s/@DOXY_VERSION@/$$DOXY_VERSION/g" \
	    < $< > $@

generated.dox: generated.dox.in
	echo "Generating $@ ..."
	CURRENT_DATE=`date "+%b %d, %Y"`; \
	GIT_REVISION=`git rev-parse --short=10 HEAD`; \
	DOXYGEN_VERSION_SHORT=`$(DOXYDOC) --version|cut -f1 -d' '`; \
	sed -e"s/@CURRENT_DATE@/$$CURRENT_DATE/g" \
	    -e"s/@GIT_REVISION@/$$GIT_REVISION/g" \
	    -e"s/@DOXYGEN_VERSION_SHORT@/$$DOXYGEN_VERSION_SHORT/g" \
	    < $< > $@

copyright.dox: copyright.dox.in
	echo "Generating $@ ..."
	YEAR=`date +%Y`; \
	sed -e"s/@YEAR@/$$YEAR/g" \
	    < $< > $@
