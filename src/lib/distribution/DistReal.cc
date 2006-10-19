#include <config.h>
#include <distribution/DistReal.h>
#include <rng/RNG.h>
#include <sarray/SArray.h>
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
using std::logic_error;


DistReal::DistReal(string const &name, unsigned int npar, Support support, 
		   bool canbound)
  : DistScalarRmath(name, npar, support, canbound, false)
{
}

double DistReal::logLikelihood(double const x,
			       vector<double const *> const &parameters) const
{
    double const *lb = lowerBound(this, parameters);
    double const *ub = upperBound(this, parameters);

    double loglik = d(x, parameters, true);
    if (lb || ub) {
	double lower = -DBL_MAX, upper = DBL_MAX;
	support(&lower, &upper, parameters);
	double plower = 0, pupper = 1;
    if (lb && ub) {
	lowerSupport(&ll, parameters, dims);
	if (y < ll)
	    return -DBL_MAX;
	upperSupport(&uu, parameters, dims); 
	if (y > uu) {
	    return -DBL_MAX;
	}
	loglik -= log(p(uu, parameters, true, false) - 
		      p(ll, parameters, true, false));
    }
    else if (lb) {
	lowerSupport(&ll, parameters, dims);
	if (y < ll)
	    return -DBL_MAX;
	loglik -= p(ll, parameters, false, true);
    }
    else if (ub) {
	upperSupport(&uu, parameters, dims); 
	if (y > uu) {
	    return -DBL_MAX;
	}
	loglik -= p(uu, parameters, true, true);
    }
    return loglik;
}

double 
DistReal::randomSample(double x, vector<double const *> const &parameters,
		       RNG *rng) const
{

    double const *bb = lowerBound(this, parameters);
    double const *ba = upperBound(this, parameters);

    if (!ba && !bb) {
	return r(parameters, rng);
    }
    else {
        double lower=-DBL_MAX, upper=DBL_MAX;	
	support(&lower, &upper, parameters);
	double plower = 0, pupper = 1;
	if (bb) {
	    plower = p(lower, parameters, true, false);
	}
	if (ba) {
	    pupper = p(upper, parameters, true, false);	    
	}
	double px = plower + rng->uniform() * (pupper - plower);
	return q(px, parameters, true, false);
    }
}
