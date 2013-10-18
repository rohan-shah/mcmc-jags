SUBDIRS = functions samplers monitors rngs

### Base module: note that the name is "basemod" not "base" to avoid
### conflict with the base package in R.

jagsmod_LTLIBRARIES = basemod.la

basemod_la_SOURCES = base.cc

basemod_la_CPPFLAGS = -I$(top_srcdir)/src/include

basemod_la_LIBADD = functions/basefunctions.la  \
        monitors/basemonitors.la                \
        samplers/basesamplers.la                \
        rngs/baserngs.la                        \
	$(top_builddir)/src/jrmath/libjrmath.la \
	$(top_builddir)/src/lib/libjags.la        

basemod_la_LDFLAGS = -module -avoid-version
if WINDOWS
basemod_la_LDFLAGS += -no-undefined
endif

### Test library 

check_LTLIBRARIES = libbasetest.la
libbasetest_la_SOURCES = test.cc test.h
libbasetest_la_LIBADD = functions/libbasefuntest.la
