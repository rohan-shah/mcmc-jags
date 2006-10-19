#include <config.h>
#include <distribution/DistDiscrete.h>
#include <rng/RNG.h>
#include "Bounds.h"

#include <stdexcept>
#include <cfloat>
#include <cmath>
#include <algorithm>

using std::max;
using std::min;
using std::string;
using std::vector;
using std::length_error;
using std::runtime_error;
using std::string;
using std::logic_error;

DistDiscrete::DistDiscrete(string const &name, unsigned int npar, 
			   Support support, bool canbound)
  : DistScalarRmath(name, npar, support, canbound, true)
{
}

double 
DistDiscrete::logLikelihood(double x,
			    vector<double const *> const &parameters) const
{
    bool lb = lowerBound(this, parameters);
    bool ub = upperBound(this, parameters);

    double loglik =  d(x, parameters, true);
    if (lb || ub) {
	double lower, upper;
	support(&lower, &upper, parameters);
	if (x < lower || x > upper) {
	    return -DBL_MAX;
	}
	double plower = 0, pupper = 1;
	if (lb && lower != -DBL_MAX) {
	    //Need to correct for discreteness
	    plower = p(lower - 1, parameters, true, false);
	}
	if (ub) {
	    pupper = p(upper, parameters, true, false); 
	}
	loglik -= log(pupper - plower);
    }
    return loglik;
}

double DistDiscrete::randomSample(vector<double const *> const &parameters,
				  RNG *rng) const
{
    bool bb = lowerBound(this, parameters);
    bool ba = upperBound(this, parameters);
  
    if (ba || bb) {
	double lower, upper;
	support(&lower, &upper, parameters);
	double plower = 0, pupper = 1;
	if (bb && lower != -DBL_MAX) {
	    plower = p(lower - 1, parameters, true, false);
	}
	if (ba) {
	    pupper = p(upper, parameters, true, false);
	}
	double u = plower + rng->uniform() * (pupper - plower);
	return q(u, parameters, true, false);
    }
    else {
	return r(parameters, rng);
    }
}
