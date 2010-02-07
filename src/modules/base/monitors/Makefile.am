noinst_LTLIBRARIES = basemonitors.la

basemonitors_la_CPPFLAGS = -I$(top_srcdir)/src/include

basemonitors_la_LDFLAGS = -no-undefined -module -avoid-version

basemonitors_la_SOURCES = TraceMonitor.cc TraceMonitorFactory.cc	\
Deviance.cc DevianceFactory.cc

noinst_HEADERS = TraceMonitor.h TraceMonitorFactory.h Deviance.h	\
DevianceFactory.h
