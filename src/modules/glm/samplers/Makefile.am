noinst_LTLIBRARIES = glmsampler.la

glmsampler_la_CPPFLAGS = -I$(top_srcdir)/src/include \
		-I$(top_srcdir)/src/modules/glm/CSparse

glmsampler_la_LDFLAGS = -no-undefined -module -avoid-version

glmsampler_la_SOURCES = ConjugateLM.cc GLMMethod.cc		\
ConjugateLMFactory.cc GLMFactory.cc Probit.cc ProbitFactory.cc	\
GLMSampler.cc ScaleMixture.cc LogitFactory.cc Logit.cc \
IWLS.cc IWLSFactory.cc

noinst_HEADERS = ConjugateLMFactory.h GLMFactory.h ConjugateLM.h	\
GLMMethod.h Probit.h ProbitFactory.h GLMSampler.h ScaleMixture.h	\
LogitFactory.h Logit.h IWLS.h IWLSFactory.h

