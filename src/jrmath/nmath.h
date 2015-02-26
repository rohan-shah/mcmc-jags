/*
 *  Mathlib : A C Library of Special Functions
 *  Copyright (C) 1998-2013  The R Core Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, a copy is available at
 *  http://www.r-project.org/Licenses/
 */

/* S Like Fortran Interface */
/* These may not be adequate everywhere. Convex had _ prepending common
   blocks, and some compilers may need to specify Fortran linkage */



/* Remap for JAGS */
#define norm_rand jags_norm_rand
#define unif_rand jags_unif_rand
#define exp_rand  jags_exp_rand

/* Private header file for use during compilation of Mathlib */
#ifndef MATHLIB_PRIVATE_H
#define MATHLIB_PRIVATE_H

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

/* Required by C99 but might be slow */
#ifdef HAVE_LONG_DOUBLE
#  define LDOUBLE long double
#else
#  define LDOUBLE double
#endif

#include <math.h>
#include <float.h> /* DBL_MIN etc */

#include <JRmath.h>

/* Used internally only */
double  jags_d1mach(int);
double	jags_gamma_cody(double);

/* Copied from R_ext/RS.h */

#ifdef HAVE_F77_UNDERSCORE
# define F77_CALL(x)    x ## _
#else
# define F77_CALL(x)    x
#endif
#define F77_NAME(x)    F77_CALL(x)
#define F77_SUB(x)     F77_CALL(x)
#define F77_COM(x)     F77_CALL(x)
#define F77_COMDECL(x) F77_CALL(x)

/* moved from dpq.h */
#ifdef HAVE_NEARYINT
# define R_forceint(x)   nearbyint()
#else
# define R_forceint(x)   round(x)
#endif
//R >= 3.1.0: # define R_nonint(x) 	  (fabs((x) - R_forceint(x)) > 1e-7)
# define R_nonint(x) 	  (fabs((x) - R_forceint(x)) > 1e-7*fmax2(1., fabs(x)))

/* Mathlib standalone */

#include <stdio.h>
#include <stdlib.h> /* for exit */
#define MATHLIB_ERROR(fmt,x)	{ printf(fmt,x); exit(1); }
#define MATHLIB_WARNING(fmt,x)		printf(fmt,x)
#define MATHLIB_WARNING2(fmt,x,x2)	printf(fmt,x,x2)
#define MATHLIB_WARNING3(fmt,x,x2,x3)	printf(fmt,x,x2,x3)
#define MATHLIB_WARNING4(fmt,x,x2,x3,x4) printf(fmt,x,x2,x3,x4)
#define MATHLIB_WARNING5(fmt,x,x2,x3,x4,x5) printf(fmt,x,x2,x3,x4,x5)

#define ISNAN(x) (isnan(x)!=0)
#define R_FINITE(x)    JR_finite(x)
int JR_finite(double);

#define ML_POSINF	(1.0 / 0.0)
#define ML_NEGINF	((-1.0) / 0.0)
#define ML_NAN		(0.0 / 0.0)

#define _(String) String

#define ML_VALID(x)	(!ISNAN(x))

#define ME_NONE		0
/*	no error */
#define ME_DOMAIN	1
/*	argument out of domain */
#define ME_RANGE	2
/*	value out of range */
#define ME_NOCONV	4
/*	process did not converge */
#define ME_PRECISION	8
/*	does not have "full" precision */
#define ME_UNDERFLOW	16
/*	and underflow occured (important for IEEE)*/

#define ML_ERR_return_NAN { ML_ERROR(ME_DOMAIN, ""); return ML_NAN; }

/* For a long time prior to R 2.3.0 ML_ERROR did nothing.
   We don't report ME_DOMAIN errors as the callers collect ML_NANs into
   a single warning.
 */
#define ML_ERROR(x, s) { \
   if(x > ME_DOMAIN) { \
       char *msg = ""; \
       switch(x) { \
       case ME_DOMAIN: \
	   msg = _("argument out of domain in '%s'\n");	\
	   break; \
       case ME_RANGE: \
	   msg = _("value out of range in '%s'\n");	\
	   break; \
       case ME_NOCONV: \
	   msg = _("convergence failed in '%s'\n");	\
	   break; \
       case ME_PRECISION: \
	   msg = _("full precision may not have been achieved in '%s'\n"); \
	   break; \
       case ME_UNDERFLOW: \
	   msg = _("underflow occurred in '%s'\n");	\
	   break; \
       } \
       MATHLIB_WARNING(msg, s); \
   } \
}

/* Wilcoxon Rank Sum Distribution */

#define WILCOX_MAX 50

#ifdef HAVE_VISIBILITY_ATTRIBUTE
# define attribute_hidden __attribute__ ((visibility ("hidden")))
#else
# define attribute_hidden
#endif

/* Formerly private part of Mathlib.h */

/* always remap internal functions */
#define bd0       	jags_bd0
#define chebyshev_eval	jags_chebyshev_eval
#define chebyshev_init	jags_chebyshev_init
#define gammalims	jags_gammalims
#define lfastchoose	jags_lfastchoose
#define lgammacor	jags_lgammacor
#define stirlerr       	jags_stirlerr

	/* Chebyshev Series */

int	attribute_hidden chebyshev_init(double*, int, double);
double	attribute_hidden chebyshev_eval(double, const double *, const int);

	/* Gamma and Related Functions */

void	attribute_hidden gammalims(double*, double*);
double	attribute_hidden lgammacor(double); /* log(gamma) correction */
double  attribute_hidden stirlerr(double);  /* Stirling expansion "error" */

double	attribute_hidden lfastchoose(double, double);

double  attribute_hidden bd0(double, double);

double	attribute_hidden dbinom_raw(double, double, double, double, int);
double	attribute_hidden dpois_raw (double, double, int);
double  attribute_hidden pnchisq_raw(double, double, double, double, double, 
				     int, Rboolean, Rboolean);
double  attribute_hidden pgamma_raw(double, double, int, int);
double	attribute_hidden pbeta_raw(double, double, double, int, int);
double  attribute_hidden qchisq_appr(double, double, double, int, int, double tol);
LDOUBLE attribute_hidden pnbeta_raw(double, double, double, double, double);
double	attribute_hidden pnbeta2(double, double, double, double, double, int, int);

int	jags_i1mach(int);

/* From toms708.c */
void attribute_hidden bratio(double a, double b, double x, double y,
	    		     double *w, double *w1, int *ierr, int log_p);


#endif /* MATHLIB_PRIVATE_H */
