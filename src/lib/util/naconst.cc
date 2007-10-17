#include <config.h>
#include <util/nainf.h>
#include <cfloat>

static double f_div(double x, double y)
{
  return x/y;
}

const double JAGS_NA = -DBL_MAX*(1-1e-15);
const double JAGS_NAN = f_div(0.0, 0.0);
const double JAGS_POSINF = f_div(1.0, 0.0);
const double JAGS_NEGINF = f_div(-1.0, 0.0);
