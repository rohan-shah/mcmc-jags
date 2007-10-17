#include <config.h>
#include <util/nainf.h>
#include <float.h>
#include <math.h>

/* We use C rather than C++ to detect non-finite values because there
   is no standard way to do it in C++.  The problems include:
   - The C99 standard defines macros "isfinite" and "isnan". However,
   the current C++ standard predates C99, so there is no guarantee that
   these exist. And if they do exist, they may be macros or functions,
   and in the latter case, may be declared inside the std namespace.
   Or not.
   - The configure script, using m4 macros borrowed from R, creates
   macros HAVE_WORKING_FINITE and HAVE_WORKING_ISFINITE. These
   are created using test functions C, but the results may not be
   valid for C++ (for example, in Solaris).
*/

#ifdef HAVE_IEEEFP_H
# include <ieeefp.h> 
#endif

int jags_finite(double x)
{
#ifdef HAVE_WORKING_ISFINITE
  return isfinite(x);
#elif HAVE_WORKING_FINITE
  return finite(x);
# else
  /* neither finite nor isfinite work */
  return (!isnan(x) & (x != JAGS_POSINF) & (x != JAGS_NEGINF));
#endif
}

int jags_isnan(double x)
{
  return (isnan(x) != 0);
}
