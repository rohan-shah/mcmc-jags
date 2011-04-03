#include <config.h>
#include "DF.h"

#include <JRmath.h>

using std::vector;

DF::DF()
    : RScalarDist("df", 2, DIST_POSITIVE)
{}

bool DF::checkParameterValue (vector<double const *> const &par) const
{
    return (*par[0] > 0 && *par[1] > 0);
}

double DF::d(double x, vector<double const *> const &par, bool give_log) const
{
    return dF(x, *par[0], *par[1], give_log);
}

double DF::p(double x, vector<double const *> const &par, bool lower, 
	     bool use_log) const
{
    return pF(x, *par[0], *par[1], lower, use_log);
}

double DF::q(double p, vector<double const *> const &par, bool lower, 
	     bool log_p) const
{
    return qF(p, *par[0], *par[1], lower, log_p);
}

double DF::r(vector<double const *> const &par, RNG *rng) const
{
    return rF(*par[0], *par[1], rng);
}

