noinst_LTLIBRARIES = cholmodmodify.la

cholmodmodify_la_SOURCES = cholmod_rowadd.c cholmod_updown.c	\
cholmod_rowdel.c

cholmodmodify_la_CPPFLAGS =				\
-I$(top_srcdir)/src/modules/glm/SSparse/UFconfig		\
-I$(top_srcdir)/src/modules/glm/SSparse/CHOLMOD/Include

cholmodmodify_la_LDFLAGS = -no-undefined -module -avoid-version

EXTRA_DIST = License.txt gpl.txt t_cholmod_updown_numkr.c t_cholmod_updown.c
