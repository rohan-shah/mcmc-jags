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
    : DistScalarRmath("dbern", 1, DIST_PROPORTION, false, true)
{}

bool 
DBern::checkParameterValue (vector<double const *> const &parameters,
			    vector<vector<unsigned int> > const &dims) const
{
    return  (PROB(parameters) >= 0.0 && PROB(parameters) <= 1.0);
}

double DBern::d(double x, vector<double const *> const &parameters,
		bool give_log) const
{
    double d = 0;
    if (x == 1)
	d = PROB(parameters);
    else if (x == 0)
	d = 1 - PROB(parameters);
  
    if (give_log) {
	return d == 0 ? JAGS_NEGINF : log(d);
    }
    else {
	return d;
    }
}

double DBern::p(double x, vector<double const *> const &parameters, bool lower,
		bool give_log) const
{
    return pbinom(x, 1, PROB(parameters), lower, give_log);
}

double DBern::q(double p, std::vector<double const *> const &parameters, 
		bool lower, bool log_p) const
{
    return qbinom(p, 1, PROB(parameters), lower, log_p);
}

double DBern::r(std::vector<double const *> const &parameters, RNG *rng) const
{
  return rng->uniform() < PROB(parameters) ? 1 : 0;
}

