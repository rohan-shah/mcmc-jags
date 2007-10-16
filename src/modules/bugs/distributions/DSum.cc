#include <config.h>
#include <util/logical.h>
#include "DSum.h"

#include <cfloat>
#include <cmath>
#include <stdexcept>

using std::vector;
using std::fabs;
using std::runtime_error;
using std::logic_error;

static inline double SUM(vector<double const *> const &par)
{
    return *par[0] + *par[1];
}

DSum::DSum()
    : DistScalar("dsum", 2, DIST_SPECIAL, false, true)
{
}

bool DSum::checkParameterValue(vector<double const *> const &par,
			       vector<vector<unsigned int> > const &dims) const
{
    return true;
}

bool DSum::checkParameterDiscrete(vector<bool> const &mask) const
{
    return allTrue(mask);
}

double DSum::logLikelihood(double x, vector<double const *> const &par,
			   double const *lower, double const *upper) const
{
    if (fabs(x - SUM(par)) > 16 * DBL_EPSILON) {
	// If this happens by accident, you have no chance of getting it right
	throw runtime_error("Inconsistent arguments for dsum");
    }
    
    return 0;
}

double DSum::randomSample(vector<double const *> const &par, 
			  double const *lower, double const *upper,
			  RNG *rng) const
{
    /* The random sample from DSum is not random at all, but
       deterministic. */
    return SUM(par);
}

double DSum::l(std::vector<double const *> const &par) const
{
    return SUM(par);
}

double DSum::u(std::vector<double const *> const &par) const
{
    return SUM(par);
}

double DSum::typicalValue(vector<double const *> const &par,
			  double const *lower, double const *upper) const
{
    return SUM(par);
}

bool DSum::isSupportFixed(vector<bool> const &fixmask) const
{
    return fixmask[0] && fixmask[1];
}

unsigned int DSum::df(vector<vector<unsigned int> > const &dims) const
{
    return 0;
}
