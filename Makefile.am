ACLOCAL_AMFLAGS = -I m4
SUBDIRS = m4 libltdl src etc win32
EXTRA_DIST = Doxyfile

win32-installer:
	make install DESTDIR=`pwd`/win32/inst
	if test -d lapack; then \
	  for f in `ls lapack/*.dll`; do \
	    ${INSTALL} $${f} win32/inst/${prefix}/bin; \
	  done; \
	  for f in `ls lapack/*.dll.a`; do \
	    ${INSTALL} $${f} win32/inst/${prefix}/lib; \
	  done; \
	fi
	makensis -DVERSION=${PACKAGE_VERSION} win32/jags.ini 
