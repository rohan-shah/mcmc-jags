jagsmod_LTLIBRARIES = cut.la

cut_la_SOURCES = cut.cc DCut.cc

cut_la_CPPFLAGS = -I$(top_srcdir)/src/include

cut_la_LIBADD = $(top_builddir)/src/lib/libjags.la 		\

cut_la_LDFLAGS = -no-undefined -module -avoid-version

noinst_HEADERS = DCut.h
