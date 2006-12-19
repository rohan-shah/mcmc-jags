#include <config.h>
#include <sarray/nainf.h>
#include <cfloat>
#include <cmath>

//For Solaris
#ifdef HAVE_IEEEFP_H
# include <ieeefp.h> 
#endif

static double f_div(double x, double y)
{
  return x/y;
}

const double JAGS_NA = -DBL_MAX*(1-1e-15);
const double JAGS_POSINF = f_div(1.0, 0.0);
const double JAGS_NEGINF = f_div(-1.0, 0.0);

bool jags_finite(double x)
{
#ifdef HAVE_WORKING_ISFINITE
  return isfinite(x);
#elif HAVE_WORKING_FINITE
  return finite(x);
# else
  /* neither finite nor isfinite work */
# ifdef _AIX
#  include <fp.h>
  return FINITE(x);
# else
  return (!jags_isnan(x) & (x != JAGS_POSINF) & (x != JAGS_NEGINF));
# endif
#endif
}


/* We don't know if isnan is a macro or a function defined in the 
   standard namespace. This works with both */
bool jags_isnan(double x)
{
  using namespace std;
  return (isnan(x) != 0);
}
