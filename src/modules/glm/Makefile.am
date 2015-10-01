SUBDIRS = CSparse SSparse samplers 

jagsmod_LTLIBRARIES = glm.la

glm_la_SOURCES = glm.cc

glm_la_CPPFLAGS = -I$(top_srcdir)/src/include	\
-I$(top_srcdir)/src/modules/glm/CSparse		\
-I$(top_srcdir)/src/modules/glm/SSparse/UFconfig \
-I$(top_srcdir)/src/modules/glm/SSparse/CHOLMOD/Include

glm_la_LIBADD = samplers/libglmsampler.la CSparse/csparse.la	\
	SSparse/ssparse.la $(top_builddir)/src/lib/libjags.la	\
	$(top_builddir)/src/jrmath/libjrmath.la @LAPACK_LIBS@	\
	@BLAS_LIBS@ 

glm_la_LDFLAGS = -module -avoid-version
if WINDOWS
glm_la_LDFLAGS += -no-undefined
else
# Adding FLIBS here allows static linking, but on Windows we don't want to
# do this as it will cause failure to build glm.dll using Rtools
glm_la_LIBADD += @FLIBS@
endif

### Test library 

check_LTLIBRARIES = libglmtest.la
libglmtest_la_SOURCES = testglm.cc testglm.h
libglmtest_la_CPPFLAGS = -I$(top_srcdir)/src/include
libglmtest_la_CXXFLAGS = $(CPPUNIT_CFLAGS)
libglmtest_la_LDFLAGS = $(CPPUNIT_LIBS)
libglmtest_la_LIBADD = samplers/libglmsamptest.la \
	samplers/libglmsampler.la \
	$(top_builddir)/src/lib/libtest.la \
	$(top_builddir)/src/lib/libjags.la \
	$(top_builddir)/src/jrmath/libjrmath.la

if WINDOWS
libglmtest_la_LDFLAGS += -no-undefined
endif

