ACLOCAL_AMFLAGS = -I m4
SUBDIRS = m4 libltdl src etc win doc
EXTRA_DIST = Doxyfile

win32-install:
	make prefix=`pwd`/win/inst32
	make install prefix=`pwd`/win/inst32
	(cd win; for f in `ls runtime32/*.dll`; do \
	    ${INSTALL} $${f} inst32/bin/; \
	  done; \
	  for f in `ls runtime32/*.dll.a`; do \
	    ${INSTALL} $${f} inst32/lib/; \
	  done;) 

win64-install:
	make prefix=`pwd`/win/inst64 
	make install prefix=`pwd`/win/inst64
	(cd win; for f in `ls runtime64/*.dll`; do \
	    ${INSTALL} $${f} inst64/bin/; \
	  done; \
	  for f in `ls runtime64/*.dll.a`; do \
	    ${INSTALL} $${f} inst64/lib/; \
	  done;) 

installer:
	(cd win && $(MAKE) $@)

docs: FORCE
	-@(cd doc && $(MAKE) $@)

FORCE:
