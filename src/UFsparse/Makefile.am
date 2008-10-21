SUBDIRS = AMD COLAMD CHOLMOD

lib_LTLIBRARIES = libufsparse.la

libufsparse_la_SOURCES = 

libufsparse_la_LIBADD = \
	AMD/libamd.la		\
        COLAMD/libcolamd.la	\
        CHOLMOD/libcholmod.la	\
	@BLAS_LIBS@
