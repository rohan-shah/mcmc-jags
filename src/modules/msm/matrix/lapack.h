#ifndef MSM_LAPACK_H_
#define MSM_LAPACK_H_
#define F77_DCOPY  dcopy
#define F77_DGESV dgesv
#define F77_DGEMM  dgemm
#define F77_DSCAL  dscal
#define F77_DLANGE dlange

extern "C" {

	
    /* LAPACK routines */

    void F77_DGESV (const int* n, const int* nrhs, double* a, 
		    const int* lda,	int* ipiv, double* b, const int* ldb,
		    int* info);
    
    double F77_DLANGE (const char *norm, const int *m, const int *n,
		       const double *a, const int *lda, double *work);
    
    /* BLAS routines */

    void F77_DGEMM (const char* transa, const char* transb, const int* m,
		    const int* n , const int* k, const double* alpha,
		    const double* A, const int* lda, 
		    const double* B, const int* ldb,
		    const double* beta, double* C, const int* ldc);

    void F77_DCOPY (const int* n, double const *X, const int* incx, double *Y,
		    const int *incy);
    
    void F77_DSCAL (const int* n, double const *alpha, double *X, 
		    const int *incx);
}

#endif /* MSM_LAPACK_H_ */