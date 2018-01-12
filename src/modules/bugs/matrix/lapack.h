#ifndef JAGS_LAPACK_H_
#define JAGS_LAPACK_H_

#define F77_DGEEV dgeev
#define F77_DSYEV dsyev
#define F77_DGESV dgesv
#define F77_DSYSV dsysv
#define F77_DPOSV dposv
/* #define F77_DGEQP3 F77_FUNC(dgeqp3,DGEQP3) */
#define F77_DPOTRF dpotrf
#define F77_DLANGE dlange
#define F77_DGEMM  dgemm
#define F77_DGEMV  dgemv
#define F77_DCOPY  dcopy
#define F77_DSCAL  dscal
#define F77_DAXPY  daxpy
#define F77_DSYR   dsyr
#define F77_DSYMM  dsymm
#define F77_DPOTRI dpotri
#define F77_DDOT   ddot
#include "mkl.h"
#endif
