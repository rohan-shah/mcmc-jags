noinst_LTLIBRARIES = cholmodcore.la

cholmodcore_la_SOURCES = cholmod_aat.c cholmod_copy.c			\
cholmod_transpose.c cholmod_add.c cholmod_dense.c cholmod_triplet.c	\
cholmod_band.c cholmod_error.c cholmod_change_factor.c			\
cholmod_factor.c cholmod_common.c cholmod_memory.c cholmod_complex.c	\
cholmod_sparse.c

cholmodcore_la_CPPFLAGS = -I$(top_srcdir)/src/modules/glm/SSparse/UFconfig \
-I$(top_srcdir)/src/modules/glm/SSparse/CHOLMOD/Include


cholmodcore_la_LDFLAGS = -no-undefined -module -avoid-version

EXTRA_DIST = License.txt lesser.txt t_cholmod_change_factor.c	\
t_cholmod_dense.c t_cholmod_transpose.c t_cholmod_triplet.c
