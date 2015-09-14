SUBDIRS = distributions functions samplers matrix 

jagsmod_LTLIBRARIES = bugs.la

bugs_la_SOURCES = bugs.cc

bugs_la_CPPFLAGS = -I$(top_srcdir)/src/include

bugs_la_LIBADD = distributions/bugsdist.la              \
        functions/bugsfunc.la                           \
        samplers/bugssampler.la                         \
        matrix/bugsmatrix.la                            \
        $(top_builddir)/src/jrmath/libjrmath.la		\
        $(top_builddir)/src/lib/libjags.la 		\
        @LAPACK_LIBS@ @BLAS_LIBS@ 

bugs_la_LDFLAGS = -module -avoid-version

if WINDOWS
bugs_la_LDFLAGS += -no-undefined
else
# Adding FLIBS here allows static linking, but on Windows we don't want to
# do this as it will cause failure to build bugs.dll using Rtools
bugs_la_LIBADD += @FLIBS@
endif

### Test library 

check_LTLIBRARIES = libbugstest.la
libbugstest_la_SOURCES = testbugs.cc testbugs.h
libbugstest_la_LIBADD = functions/libbugsfuntest.la	\
	distributions/libbugsdisttest.la 		\
	$(top_builddir)/src/lib/libtest.la		\
	$(top_builddir)/src/lib/libjags.la
libbugstest_la_CPPFLAGS = -I$(top_srcdir)/src/include
libbugstest_la_CXXFLAGS = $(CPPUNIT_CFLAGS)
