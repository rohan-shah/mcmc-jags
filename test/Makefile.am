# Rules for the test code (use `make check` to execute)
TESTS = modules
check_PROGRAMS = $(TESTS)

modules_SOURCES = modules.cc 
modules_CXXFLAGS = $(CPPUNIT_CFLAGS)
modules_LDFLAGS = $(CPPUNIT_LIBS) -ldl

modules_LDADD = $(LIBLTDL) ${top_builddir}/src/lib/libjags.la \
	-dlpreopen ${top_builddir}/src/modules/base/basemod.la \
	-dlpreopen ${top_builddir}/src/modules/bugs/bugs.la \
	$(top_builddir)/src/modules/base/libbasetest.la \
	$(top_builddir)/src/modules/bugs/libbugstest.la 

modules_CPPFLAGS = -I$(top_srcdir)/src/include	\
	-I$(top_srcdir)/src/modules \
	$(LTDLINCL)

