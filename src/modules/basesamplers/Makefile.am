jagsmod_LTLIBRARIES = basesamplers.la

basesamplers_la_SOURCES = DiscreteSlicer.cc FiniteFactory.cc	\
FiniteMethod.cc RealSliceSampler.cc SliceFactory.cc module.cc

basesamplers_la_CPPFLAGS = -I$(top_srcdir)/src/include

basesamplers_la_LDFLAGS = -no-undefined -module -avoid-version

basesamplers_la_LIBADD = $(top_builddir)/src/lib/libjags.la

noinst_HEADERS = DiscreteSlicer.h FiniteFactory.h		\
FiniteMethod.h RealSliceSampler.h SliceFactory.h 

