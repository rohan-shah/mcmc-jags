noinst_LTLIBRARIES = cholmodmatrix.la

cholmodmatrix_la_SOURCES = cholmod_drop.c cholmod_scale.c		\
cholmod_submatrix.c cholmod_horzcat.c cholmod_sdmult.c			\
cholmod_symmetry.c cholmod_norm.c cholmod_ssmult.c cholmod_vertcat.c

cholmodmatrix_la_CPPFLAGS =				\
-I$(top_srcdir)/src/modules/glm/SSparse/UFconfig		\
-I$(top_srcdir)/src/modules/glm/SSparse/CHOLMOD/Include

cholmodmatrix_la_LDFLAGS = -no-undefined -module -avoid-version

EXTRA_DIST = License.txt gpl.txt t_cholmod_sdmult.c
