SUBDIRS = Check Cholesky Core Include MatrixOps Modify Supernodal 

noinst_LTLIBRARIES = cholmod.la

cholmod_la_SOURCES =

cholmod_la_LIBADD = Cholesky/cholmodcholesky.la 	\
	Core/cholmodcore.la \
	MatrixOps/cholmodmatrix.la \
	Modify/cholmodmodify.la \
	Supernodal/cholmodsupernodal.la \
	Check/cholmodcheck.la

cholmod_la_LDFLAGS = -no-undefined -module -avoid-version

DIST_EXTRA = README.txt
