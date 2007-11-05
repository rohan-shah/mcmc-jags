#include <config.h>
#include "DPois.h"

#include <limits.h>
#include <algorithm>

#include <JRmath.h>

using std::vector;
using std::max;

#define LAMBDA(par) (*par[0])

DPois::DPois()
    : DistScalarRmath("dpois", 1, DIST_POSITIVE, true, true)
{}

bool DPois::checkParameterValue (vector<double const *> const &par,
				 vector<vector<unsigned int> > const &dims) 
  const
{
    return (LAMBDA(par) >= 0);
}

double
DPois::d(double x, vector<double const *> const &par, bool give_log) const
{
    return dpois(x, LAMBDA(par), give_log);
}

double
DPois::p(double q, vector<double const *> const &par, bool lower, bool give_log)
    const
{
    return ppois(q, LAMBDA(par), lower, give_log);
}

double 
DPois::q(double p, vector<double const *> const &par, bool lower, bool log_p)
    const
{
    return qpois(p, LAMBDA(par), lower, log_p);
}

double DPois::r(vector<double const *> const &par, RNG *rng) const
{
    return rpois(LAMBDA(par), rng);
}
