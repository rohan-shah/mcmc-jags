#ifndef JAGS_LAPACK_H_
#define JAGS_LAPACK_H_

#define F77_DGEEV dgeev_
#define F77_DSYEV dsyev_
#define F77_DGESV dgesv_
#define F77_DSYSV dsysv_
#define F77_DPOSV dposv_
/* #define F77_DGEQP3 F77_FUNC(dgeqp3,DGEQP3) */
#define F77_DPOTRF dpotrf_
#define F77_DLANGE dlange_
#define F77_DGEMM  dgemm_
#define F77_DGEMV  dgemv_
#define F77_DCOPY  dcopy_
#define F77_DSCAL  dscal_
#define F77_DAXPY  daxpy_
#define F77_DSYR   dsyr_
#define F77_DSYMM  dsymm_
#define F77_DPOTRI dpotri_
#define F77_DDOT   ddot_
    
extern "C" {
/*
  void F77_DGEEV (const char* jobvl, const char* jobvr,
  const int* n, double* a, const int* lda,
  double* wr, double* wi, double* vl, const int* ldvl,
  double* vr, const int* ldvr,
  double* work, const int* lwork, int* info);
*/
    void F77_DSYEV (const char* jobz, const char* uplo,
		    const int* n, double* a, const int* lda,
		    double* w, 
		    double* work, const int* lwork, int* info);
	
	
    void F77_DGESV (const int* n, const int* nrhs, double* a, 
		    const int* lda,	int* ipiv, double* b, const int* ldb,
		    int* info);

    void F77_DSYSV (const char *uplo, const int *n, const int *nrhs,
		    double *a, const int *lda, int *ipiv, double *b,
		    const int *ldb, double *work, const int *lwork,
		    int *info);
	
    void F77_DPOSV (const char* uplo, const int *n, const int *nrhs,
		    double* a, const int *lda, double* b, const int* ldb,
		    int* info);

    void F77_DPOTRF (const char *uplo, const int *n, double *a,
		     const int *lda, const int *info);

    void F77_DPOTRI (const char *uplo, const int *n, double *a,
		     const int *lda, const int *info);

    double F77_DLANGE (const char *norm, const int *m, const int *n,
		       const double *a, const int *lda, double *work);

    /* BLAS routines */

    void F77_DSYMM (const char *side, const char *uplo, const int *m,
		    const int *n, const double *alpha,
		    const double *a, const int *lda,
		    const double *b, const int *ldb,
		    const double *beta, double *c, const int *ldc);
        
    void F77_DGEMV (const char *trans, const int *m, const int *n,
		    const double *alpha, const double *a, const int *lda,
		    const double *x, const int *incx, const double *beta,
		    double *y, const int *incy);
    
    void F77_DGEMM (const char* transa, const char* transb, const int* m,
		    const int* n , const int* k, const double* alpha,
		    const double* A, const int* lda, 
		    const double* B, const int* ldb,
		    const double* beta, double* C, const int* ldc);
 
    void F77_DSYR (const char *uplo, const int *n, const double *alpha,
		   const double *x, const int *incx,
		   double *a, const int *lda);

    void F77_DAXPY (const int *n, const double *alpha,
		    const double *dx, const int *incx,
		    double *dy, const int *incy);

    void F77_DCOPY (const int* n, double const *X, const int* incx, double *Y,
		    const int *incy);
    
    void F77_DSCAL (const int* n, double const *alpha, double *X, 
		    const int *incx);

    double F77_DDOT (const int* n, double const *X, const int *incx, 
		     double const *Y, const int *incy);

    void F77_DTRTRI (const char *uplo, const char *diag,
		     const int *n, double *a, const int *lda, const int *info);

    void F77_DTRMM(const char *side, const char *uplo, const char *transa,
		   const char *diag, const int *m, const int *n,
		   const double *alpha, const double *a, const int *lda,
		   double *b, const int *ldb);

    void F77_DSYRK(const char *uplo, const char *trans, const int *n,
		   const int *k,
		   const double *alpha, const double *a, const int *lda,
		   const double *beta, double *c, const int *ldc);
}

#endif
