noinst_LTLIBRARIES = libcholmod.la

libcholmod_la_SOURCES =		\
Core/cholmod_aat.c		\
Core/cholmod_add.c		\
Core/cholmod_band.c		\
Core/cholmod_change_factor.c	\
Core/cholmod_common.c		\
Core/cholmod_complex.c		\
Core/cholmod_copy.c		\
Core/cholmod_dense.c		\
Core/cholmod_error.c		\
Core/cholmod_factor.c		\
Core/cholmod_memory.c		\
Core/cholmod_sparse.c		\
Core/cholmod_transpose.c	\
Core/cholmod_triplet.c		\
Check/cholmod_check.c		\
Check/cholmod_read.c		\
Check/cholmod_write.c		\
Cholesky/cholmod_amd.c		\
Cholesky/cholmod_analyze.c	\
Cholesky/cholmod_colamd.c	\
Cholesky/cholmod_etree.c	\
Cholesky/cholmod_factorize.c	\
Cholesky/cholmod_postorder.c	\
Cholesky/cholmod_rcond.c	\
Cholesky/cholmod_resymbol.c	\
Cholesky/cholmod_rowcolcounts.c	\
Cholesky/cholmod_rowfac.c	\
Cholesky/cholmod_solve.c	\
Cholesky/cholmod_spsolve.c	\
MatrixOps/cholmod_drop.c	\
MatrixOps/cholmod_horzcat.c	\
MatrixOps/cholmod_norm.c	\
MatrixOps/cholmod_scale.c	\
MatrixOps/cholmod_sdmult.c	\
MatrixOps/cholmod_ssmult.c	\
MatrixOps/cholmod_submatrix.c	\
MatrixOps/cholmod_symmetry.c	\
MatrixOps/cholmod_vertcat.c	\
Partition/cholmod_camd.c	\
Partition/cholmod_ccolamd.c	\
Partition/cholmod_csymamd.c	\
Partition/cholmod_metis.c	\
Partition/cholmod_nesdis.c	\
Modify/cholmod_rowadd.c		\
Modify/cholmod_rowdel.c		\
Modify/cholmod_updown.c		\
Supernodal/cholmod_super_numeric.c	\
Supernodal/cholmod_super_solve.c	\
Supernodal/cholmod_super_symbolic.c

noinst_HEADERS = \
Include/cholmod_blas.h		\
Include/cholmod_complexity.h	\
Include/cholmod_internal.h	\
Include/cholmod_io64.h		\
Include/cholmod_template.h

EXTRA_DIST = \
Cholesky/t_cholmod_lsolve.c		\
Cholesky/t_cholmod_ltsolve.c		\
Cholesky/t_cholmod_rowfac.c		\
Cholesky/t_cholmod_solve.c		\
Core/t_cholmod_change_factor.c		\
Core/t_cholmod_dense.c			\
Core/t_cholmod_transpose.c		\
Core/t_cholmod_triplet.c		\
MatrixOps/t_cholmod_sdmult.c		\
Modify/t_cholmod_updown.c		\
Modify/t_cholmod_updown_numkr.c		\
Supernodal/t_cholmod_super_numeric.c	\
Supernodal/t_cholmod_super_solve.c	\
Check/lesser.txt	\
Check/License.txt	\
Cholesky/lesser.txt	\
Cholesky/License.txt	\
Core/lesser.txt		\
Core/License.txt	\
MatrixOps/gpl.txt	\
MatrixOps/License.txt	\
Modify/gpl.txt		\
Modify/License.txt	\
Partition/lesser.txt	\
Partition/License.txt	\
Supernodal/gpl.txt	\
Supernodal/License.txt


libcholmod_la_CPPFLAGS = -I$(top_srcdir)/src/include/UFsparse \
			 -I$(srcdir)/Include -DNPARTITION

libcholmod_la_LDFLAGS = -no-undefined 

