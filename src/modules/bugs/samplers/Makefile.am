# SUBDIRS = cj

noinst_LTLIBRARIES = bugssampler.la

bugssampler_la_CPPFLAGS = -I$(top_srcdir)/src/include	\
-I$(top_srcdir)/src/modules/bugs/distributions		\
-I$(top_srcdir)/src/modules/bugs/matrix

bugssampler_la_LDFLAGS = -no-undefined -module -avoid-version

# bugssampler_la_LIBADD = cj/bugscj.la

bugssampler_la_SOURCES = Censored.cc ConjugateGamma.cc		\
ConjugateWishart.cc ConjugateBeta.cc ConjugateMNormal.cc	\
ConjugateDirichlet.cc ConjugateNormal.cc DSumFactory.cc		\
ConjugateFactory.cc ConjugateSampler.cc RWDSum.cc RealDSum.cc	\
DiscreteDSum.cc MNormal.cc MNormalFactory.cc ConjugateMethod.cc	\
Dirichlet.cc DirichletFactory.cc TruncatedGamma.cc DMultiDSum.cc \
ShiftedCount.cc ShiftedMultinomial.cc

noinst_HEADERS = Censored.h ConjugateFactory.h ConjugateNormal.h	\
ConjugateBeta.h ConjugateGamma.h ConjugateSampler.h DSumFactory.h	\
ConjugateDirichlet.h ConjugateMNormal.h ConjugateWishart.h		\
RWDSum.h RealDSum.h DiscreteDSum.h MNormal.h MNormalFactory.h		\
ConjugateMethod.h Dirichlet.h DirichletFactory.h TruncatedGamma.h	\
DMultiDSum.h ShiftedCount.h ShiftedMultinomial.h

