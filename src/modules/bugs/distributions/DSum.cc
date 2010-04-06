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

static double SUM(vector<double const *> const &par)
{
    double ans = 0;
    for (unsigned int i = 0; i < par.size(); ++i) {
	ans += *par[i];
    }
    return ans;
}

DSum::DSum()
    : ScalarDist("dsum", 0, DIST_SPECIAL)
{
}

bool DSum::isDiscreteValued(vector<bool> const &mask) const
{
    return allTrue(mask);
}

double DSum::logLikelihood(double x, vector<double const *> const &par,
			   double const *lower, double const *upper) const
{
    if (fabs(x - SUM(par)) > sqrt(DBL_EPSILON)) {
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
    return allTrue(fixmask);
}

unsigned int DSum::df() const
{
    return 0;
}

bool DSum::checkParameterValue(vector<double const *> const &params) const
{
    return true;
}

bool DSum::checkParameterDiscrete(vector<double const *> const &mask) const
{
    for (unsigned int i = 1; i < mask.size(); ++i) {
	if (mask[i] != mask[0])
	    return false;
    }
    return true;
}
