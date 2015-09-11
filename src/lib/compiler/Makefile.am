noinst_LTLIBRARIES = libcompiler.la
AM_YFLAGS = -d
BUILT_SOURCES = parser.hh

libcompiler_la_CPPFLAGS = -I$(top_srcdir)/src/include

libcompiler_la_SOURCES = parser.yy scanner.ll Compiler.cc	\
LogicalFactory.cc ParseTree.cc Counter.cc CounterTab.cc		\
MixtureFactory.cc MixCompiler.cc NodeFactory.cc ObsFuncTab.cc

noinst_HEADERS = parser.hh parser_extra.h remap.h MixCompiler.h

MAINTAINERCLEANFILES = parser.cc scanner.cc parser.hh
