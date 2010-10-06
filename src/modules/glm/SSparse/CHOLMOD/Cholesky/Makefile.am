noinst_LTLIBRARIES = cholmodcholesky.la

cholmodcholesky_la_SOURCES = cholmod_amd.c cholmod_analyze.c	\
cholmod_colamd.c cholmod_etree.c cholmod_factorize.c		\
cholmod_postorder.c cholmod_rcond.c cholmod_resymbol.c		\
cholmod_rowcolcounts.c cholmod_rowfac.c cholmod_solve.c		\
cholmod_spsolve.c 

cholmodcholesky_la_CPPFLAGS = \
-I$(top_srcdir)/src/modules/glm/SSparse/UFconfig	\
-I$(top_srcdir)/src/modules/glm/SSparse/CHOLMOD/Include	\
-I$(top_srcdir)/src/modules/glm/SSparse/AMD/Include		\
-I$(top_srcdir)/src/modules/glm/SSparse/COLAMD/Include

cholmodcholesky_la_LDFLAGS = -no-undefined -module -avoid-version

EXTRA_DIST = License.txt lesser.txt t_cholmod_lsolve.c		\
t_cholmod_ltsolve.c t_cholmod_rowfac.c t_cholmod_solve.c
