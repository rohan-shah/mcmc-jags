SUBDIRS = config AMD CAMD CCOLAMD COLAMD CHOLMOD 

noinst_LTLIBRARIES = ssparse.la

ssparse_la_SOURCES =

ssparse_la_LIBADD = AMD/amd.la CAMD/camd.la CCOLAMD/ccolamd.la	\
COLAMD/colamd.la CHOLMOD/cholmod.la config/config.la

ssparse_la_LDFLAGS = -no-undefined -module -avoid-version

