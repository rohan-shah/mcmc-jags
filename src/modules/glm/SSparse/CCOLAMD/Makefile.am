noinst_LTLIBRARIES = ccolamd.la

ccolamd_la_SOURCES = Source/ccolamd.c

ccolamd_la_CPPFLAGS = -I$(top_srcdir)/src/modules/glm/SSparse/config \
-I$(top_srcdir)/src/modules/glm/SSparse/CCOLAMD/Include

noinst_HEADERS = Include/ccolamd.h 

ccolamd_la_LDFLAGS = -no-undefined -module -avoid-version

DIST_EXTRA = README.txt
