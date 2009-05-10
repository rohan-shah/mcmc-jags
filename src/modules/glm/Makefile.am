SUBDIRS = CSparse samplers 

jagsmod_LTLIBRARIES = glm.la

glm_la_SOURCES = glm.cc

glm_la_CPPFLAGS = -I$(top_srcdir)/src/include

glm_la_LIBADD = samplers/glmsampler.la 	\
	CSparse/csparse.la

# glm_la_LIBADD = samplers/glmsampler.la            \
#        $(top_builddir)/src/CSparse/libcsparse.la

glm_la_LDFLAGS = -no-undefined -module -avoid-version

