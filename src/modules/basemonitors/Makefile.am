jagsmod_LTLIBRARIES = basemonitors.la

basemonitors_la_CPPFLAGS = -I$(top_srcdir)/src/include

basemonitors_la_LDFLAGS = -no-undefined -module -avoid-version

basemonitors_la_LIBADD = $(top_builddir)/src/lib/libjags.la

basemonitors_la_SOURCES = basemonitors.cc TraceMonitor.cc	\
TraceMonitorFactory.cc

noinst_HEADERS = TraceMonitor.h TraceMonitorFactory.h
