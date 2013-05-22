noinst_LTLIBRARIES = mixsamp.la

mixsamp_la_CPPFLAGS = -I$(top_srcdir)/src/include 

mixsamp_la_LDFLAGS = -no-undefined -module -avoid-version

mixsamp_la_SOURCES = DirichletInfo.cc NormMix.cc MixSamplerFactory.cc	\
 DirichletCat.cc DirichletCatFactory.cc CatDirichlet.cc LDA.cc		\
LDAFactory.cc

noinst_HEADERS = DirichletInfo.h NormMix.h MixSamplerFactory.h	\
 DirichletCat.h DirichletCatFactory.h CatDirichlet.h LDA.h	\
LDAFactory.h
