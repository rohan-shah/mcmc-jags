ACLOCAL_AMFLAGS = -I m4
SUBDIRS = m4 libltdl src etc win doc
EXTRA_DIST = Doxyfile

win-installer:
	make prefix=`pwd`/win/inst
	make install prefix=`pwd`/win/inst
	(cd win; for f in `ls runtime/*.dll`; do \
	    ${INSTALL} $${f} inst/bin; \
	  done; \
	  for f in `ls runtime/*.dll.a`; do \
	    ${INSTALL} $${f} inst/lib; \
	  done;) 
	makensis -DVERSION=${PACKAGE_VERSION} win/jags.nsi

docs: FORCE
	-@(cd doc && $(MAKE) $@)

FORCE:
