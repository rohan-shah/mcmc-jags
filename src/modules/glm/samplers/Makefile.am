noinst_LTLIBRARIES = libglmsampler.la

libglmsampler_la_CPPFLAGS = -I$(top_srcdir)/src/include \
		-I$(top_srcdir)/src/modules/glm/CSparse \
		-I$(top_srcdir)/src/modules/glm/SSparse/UFconfig \
		-I$(top_srcdir)/src/modules/glm/SSparse/CHOLMOD/Include

libglmsampler_la_SOURCES = GLMFactory.cc GLMSampler.cc GLMMethod.cc	\
 LinearFactory.cc LinearGibbsFactory.cc Linear.cc BinaryFactory.cc	\
 KS.cc AlbertChibFactory.cc AlbertChibGibbsFactory.cc AlbertChib.cc	\
 IWLSFactory.cc IWLS.cc LGMix.cc AuxMixPoisson.cc AuxMixBinomial.cc	\
 AMMethod.cc AMFactory.cc Outcome.cc NormalLinear.cc BinaryProbit.cc	\
 BinaryLogit.cc Classify.cc IWLSOutcome.cc HolmesHeld.cc HolmesHeldFactory.cc

noinst_HEADERS = GLMFactory.h GLMSampler.h GLMMethod.h			\
  LinearFactory.h LinearGibbsFactory.h Linear.h BinaryFactory.h KS.h	\
  AlbertChibFactory.h AlbertChibGibbsFactory.h AlbertChib.h		\
  IWLSFactory.h IWLS.h LGMix.h AuxMixPoisson.h AuxMixBinomial.h		\
  AMMethod.h AMFactory.h Outcome.h NormalLinear.h BinaryProbit.h	\
  BinaryLogit.h Classify.h IWLSOutcome.h HolmesHeld.h HolmesHeldFactory.h

check_LTLIBRARIES = libglmsamptest.la
libglmsamptest_la_SOURCES = testglmsamp.cc testglmsamp.h
libglmsamptest_la_CPPFLAGS = -I$(top_srcdir)/src/include
libglmsamptest_la_CXXFLAGS = $(CPPUNIT_CFLAGS)
