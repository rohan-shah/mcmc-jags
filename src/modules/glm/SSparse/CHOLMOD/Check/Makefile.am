noinst_LTLIBRARIES = cholmodcheck.la

cholmodcheck_la_SOURCES = cholmod_check.c  cholmod_read.c  cholmod_write.c

cholmodcheck_la_CPPFLAGS = -I$(top_srcdir)/src/modules/glm/SSparse/UFconfig \
-I$(top_srcdir)/src/modules/glm/SSparse/CHOLMOD/Include

cholmodcheck_la_LDFLAGS = -no-undefined -module -avoid-version

EXTRA_DIST = License.txt lesser.txt 
