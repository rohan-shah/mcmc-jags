noinst_LTLIBRARIES = glmsampler.la

glmsampler_la_CPPFLAGS = -I$(top_srcdir)/src/include \
		-I$(top_srcdir)/src/modules/glm/CSparse

glmsampler_la_LDFLAGS = -no-undefined -module -avoid-version

glmsampler_la_SOURCES = GLMFactory.cc GLMSampler.cc GLMMethod.cc	\
LinearFactory.cc LinearGibbsFactory.cc Linear.cc BinaryFactory.cc	\
BinaryGLM.cc KS.cc AlbertChibFactory.cc AlbertChibGibbsFactory.cc	\
AlbertChib.cc HolmesHeldFactory.cc HolmesHeld.cc IWLSFactory.cc		\
IWLS.cc

noinst_HEADERS = GLMFactory.h GLMSampler.h GLMMethod.h LinearFactory.h	\
LinearGibbsFactory.h Linear.h BinaryFactory.h BinaryGLM.h KS.h		\
AlbertChibFactory.h AlbertChibGibbsFactory.h AlbertChib.h		\
HolmesHeldFactory.h HolmesHeld.h IWLSFactory.h IWLS.h
