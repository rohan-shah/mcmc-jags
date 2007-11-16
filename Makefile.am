ACLOCAL_AMFLAGS = -I m4
SUBDIRS = m4 libltdl src etc win32
EXTRA_DIST = Doxyfile

win32-installer:
	make prefix=`pwd`/win32/inst
	make install prefix=`pwd`/win32/inst
	(cd win32; if test -d lapack; then \
	  for f in `ls lapack/*.dll`; do \
	    ${INSTALL} $${f} inst/${prefix}/bin; \
	  done; \
	  for f in `ls lapack/*.dll.a`; do \
	    ${INSTALL} $${f} inst/${prefix}/lib; \
	  done; \
	fi)
	makensis -DVERSION=${PACKAGE_VERSION} win32/jags.ini 
