noinst_LTLIBRARIES = basemonitors.la

basemonitors_la_CPPFLAGS = -I$(top_srcdir)/src/include

basemonitors_la_LDFLAGS = -no-undefined -module -avoid-version

basemonitors_la_SOURCES = TraceMonitor.cc TraceMonitorFactory.cc	\
MeanMonitor.cc MeanMonitorFactory.cc

noinst_HEADERS = TraceMonitor.h TraceMonitorFactory.h MeanMonitor.h	\
MeanMonitorFactory.h
