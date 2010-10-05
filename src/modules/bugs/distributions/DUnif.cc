#include <config.h>
#include "DUnif.h"

#include <cmath>
#include <rng/RNG.h>
#include <util/nainf.h>

using std::vector;
using std::log;

#define LOWER(par) (*par[0])
#define UPPER(par) (*par[1])

DUnif::DUnif()
    : ScalarDist("dunif", 2, DIST_SPECIAL)
{}

bool  DUnif::checkParameterValue (vector<double const *> const &par) const
{
    return (LOWER(par) < UPPER(par));
}

double DUnif::logLikelihood(double x, vector<double const *> const &par,
			    double const *lower, double const *upper) const
{
    if (x < LOWER(par) || x > UPPER(par))
	return JAGS_NEGINF;
    else
	return log(UPPER(par) - LOWER(par));
}

double DUnif::randomSample(vector<double const *> const &par, 
			   double const *lower, double const *upper,
			   RNG *rng) const
{
    return LOWER(par) + rng->uniform() * (UPPER(par) - LOWER(par));
}

double DUnif::typicalValue(vector<double const *> const &par,
			   double const *lower, double const *upper) const
{
    return (LOWER(par) + UPPER(par))/2;
}

double DUnif::l(vector<double const*> const &par) const
{
    return LOWER(par);
}

double DUnif::u(vector<double const*> const &par) const
{
    return UPPER(par);
}

bool DUnif::isSupportFixed(vector<bool> const &fixmask) const
{
    return fixmask[0] && fixmask[1]; //Lower and upper bounds fixed
}
