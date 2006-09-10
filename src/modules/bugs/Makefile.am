SUBDIRS = distributions functions samplers matrix 
INCLUDES = -I $(top_srcdir)/src/include

jagsmod_LTLIBRARIES = bugs.la

bugs_la_SOURCES = bugs.cc

bugs_la_LIBADD = distributions/bugsdist.la               \
        functions/bugsfunc.la                           \
        samplers/bugssampler.la                         \
        matrix/bugsmatrix.la                            \
        $(top_builddir)/src/jrmath/libjrmath.la		\
        $(top_builddir)/src/lib/libjags.la 

bugs_la_LDFLAGS = -no-undefined -module -avoid-version
