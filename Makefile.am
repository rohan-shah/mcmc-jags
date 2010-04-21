ACLOCAL_AMFLAGS = -I m4
SUBDIRS = m4 libltdl src etc win doc
EXTRA_DIST = Doxyfile

win:
	make prefix=`pwd`/win/inst
	make install prefix=`pwd`/win/inst
	(cd win; for f in `ls runtime/*.dll`; do \
	    ${INSTALL} $${f} inst/bin; \
	  done; \
	  for f in `ls runtime/*.dll.a`; do \
	    ${INSTALL} $${f} inst/lib; \
	  done;) 

win32-installer: win
	makensis -DVERSION=${PACKAGE_VERSION} win/jags.nsi

win64-installer: win
	makensis -DVERSION=${PACKAGE_VERSION} win/jags64.nsi

docs: FORCE
	-@(cd doc && $(MAKE) $@)

FORCE:
