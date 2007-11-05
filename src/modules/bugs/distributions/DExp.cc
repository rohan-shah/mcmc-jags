#include <config.h>
#include "DExp.h"

#include <cmath>
#include <algorithm>

#include <JRmath.h>

using std::max;
using std::vector;

DExp::DExp()
    : DistScalarRmath("dexp", 1, DIST_POSITIVE, true, false)
{}

/* R functions are parameterized in terms of scale = 1/rate */
static inline double SCALE(vector<double const *> const &par)
{
    return 1 /(*par[0]);
}

bool DExp::checkParameterValue (vector<double const *> const &par,
				vector<vector<unsigned int> > const &dims) const
{
  return (*par[0] > 0);
}

double
DExp::d(double x, vector<double const *> const &par, bool give_log) 
    const
{
    return dexp(x, SCALE(par), give_log);
}

double 
DExp::p(double q, vector<double const *> const &par, bool lower, 
	bool log_p) const
{
    return pexp(q, SCALE(par), lower, log_p);
}

double 
DExp::q(double p, vector<double const *> const &par, bool lower,  
	bool log_p) const
{
    return qexp(p, SCALE(par), lower, log_p);
}

double DExp::r(vector<double const *> const &par, RNG *rng) const
{
    return rexp(SCALE(par), rng);
}

