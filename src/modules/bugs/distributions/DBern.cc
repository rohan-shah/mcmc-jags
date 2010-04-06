#include <config.h>
#include "DBern.h"
#include <rng/RNG.h>
#include <util/nainf.h>

#include <cmath>
#include <algorithm>

#include <JRmath.h>

using std::vector;
using std::min;
using std::max;

#define PROB(par) (*par[0])

DBern::DBern()
    : ScalarDist("dbern", 1, DIST_PROPORTION)
{}

bool 
DBern::checkParameterValue (vector<double const *> const &parameters) const
{
    return  (PROB(parameters) >= 0.0 && PROB(parameters) <= 1.0);
}

double DBern::logLikelihood(double x, vector<double const *> const &parameters,
			    double const *lbound, double const *ubound) const 
{
    double d = 0;
    if (x == 1)
	d = PROB(parameters);
    else if (x == 0)
	d = 1 - PROB(parameters);
    
    return d == 0 ? JAGS_NEGINF : log(d);
}

double DBern::randomSample(vector<double const *> const &parameters, 
			   double const *lbound, double const *ubound,
			   RNG *rng) const
{
    return rng->uniform() < PROB(parameters) ? 1 : 0;
}

double DBern::typicalValue(vector<double const *> const &parameters,
			   double const *lbound, double const *ubound) const
{
    return PROB(parameters) > 0.5 ? 1 : 0;
}

bool DBern::canBound() const
{
    return false;
}

bool DBern::isDiscreteValued(std::vector<bool> const &mask) const
{
    return true;
}
