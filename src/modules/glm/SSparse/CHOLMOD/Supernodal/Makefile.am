noinst_LTLIBRARIES = cholmodsupernodal.la

cholmodsupernodal_la_SOURCES = cholmod_super_numeric.c	\
cholmod_super_symbolic.c cholmod_super_solve.c 

cholmodsupernodal_la_CPPFLAGS =			\
-I$(top_srcdir)/src/modules/glm/SSparse/UFconfig	\
-I$(top_srcdir)/src/modules/glm/SSparse/CHOLMOD/Include

cholmodsupernodal_la_LDFLAGS = -no-undefined -module -avoid-version

EXTRA_DIST = License.txt gpl.txt t_cholmod_super_solve.c	\
t_cholmod_super_numeric.c
