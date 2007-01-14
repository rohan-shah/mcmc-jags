SUBDIRS = distributions samplers
INCLUDES = -I $(top_srcdir)/src/include

jagsmod_LTLIBRARIES = mix.la

mix_la_SOURCES = mix.cc

mix_la_LIBADD = distributions/mixdist.la               \
        $(top_builddir)/src/jrmath/libjrmath.la		\
        $(top_builddir)/src/lib/libjags.la 

mix_la_LDFLAGS = -no-undefined -module -avoid-version
