ACLOCAL_AMFLAGS = -I m4
SUBDIRS = m4 libltdl src etc win doc
EXTRA_DIST = Doxyfile

win-install:
	make prefix=`pwd`/win/inst
	make install prefix=`pwd`/win/inst
	(cd win; for f in `ls runtime/*.dll`; do \
	    ${INSTALL} $${f} inst/bin; \
	  done; \
	  for f in `ls runtime/*.dll.a`; do \
	    ${INSTALL} $${f} inst/lib; \
	  done;) 

win32-installer: win-install
	(cd win && $(MAKE) $@)

win64-installer: win-install
	(cd win && $(MAKE) $@)

docs: FORCE
	-@(cd doc && $(MAKE) $@)

FORCE:
