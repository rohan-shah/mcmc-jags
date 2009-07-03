SUBDIRS = CSparse samplers 

jagsmod_LTLIBRARIES = glm.la

glm_la_SOURCES = glm.cc

glm_la_CPPFLAGS = -I$(top_srcdir)/src/include \
                  -I$(top_srcdir)/src/modules/glm/CSparse

glm_la_LIBADD = samplers/glmsampler.la 	\
	CSparse/csparse.la \
	$(top_builddir)/src/lib/libjags.la

glm_la_LDFLAGS = -no-undefined -module -avoid-version

