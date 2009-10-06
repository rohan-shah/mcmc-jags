noinst_LTLIBRARIES = glmsampler.la

glmsampler_la_CPPFLAGS = -I$(top_srcdir)/src/include \
		-I$(top_srcdir)/src/modules/glm/CSparse

glmsampler_la_LDFLAGS = -no-undefined -module -avoid-version

glmsampler_la_SOURCES = ConjugateLM.cc GLMMethod.cc ConjugateLMFactory.cc GLMFactory.cc GLMSampler.cc TruncatedNormal.cc

noinst_HEADERS = ConjugateLMFactory.h GLMFactory.h GLMSampler.h	ConjugateLM.h GLMMethod.h TruncatedNormal.h

