SUBDIRS = functions samplers monitors rngs

jagsmod_LTLIBRARIES = base.la

base_la_SOURCES = base.cc

base_la_CPPFLAGS = -I$(top_srcdir)/src/include

base_la_LIBADD = functions/basefunctions.la       \
        monitors/basemonitors.la                 \
        samplers/basesamplers.la                  \
        rngs/baserngs.la                        \
        $(top_builddir)/src/jrmath/libjrmath.la   \
        $(top_builddir)/src/lib/libjags.la        

base_la_LDFLAGS = -no-undefined -module -avoid-version

