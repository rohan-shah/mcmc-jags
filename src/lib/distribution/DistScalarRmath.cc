#include <config.h>
#include <distribution/DistScalarRmath.h>
#include <rng/RNG.h>
#include <sarray/util.h>
#include <sarray/nainf.h>

#include "Bounds.h"

#include <stdexcept>
#include <cmath>
#include <algorithm>

using std::max;
using std::min;
using std::string;
using std::vector;
using std::length_error;
using std::logic_error;

DistScalarRmath::DistScalarRmath(string const &name, unsigned int npar, Support support, 
		       bool canbound, bool discrete)
  
  : DistScalar(name, npar, support, canbound, discrete), 
    _support(support)
{
}

double 
DistScalarRmath::typicalValue(vector<double const *> const &parameters) const
{

    double const *bb = lowerBound(this, parameters);
    double const *ba = upperBound(this, parameters);

    double y;
    if (!ba && !bb) {
      y = q(0.5, parameters, true, false);
    }
    else {
        double lower = 0, upper = 0;
	support(&lower, &upper, parameters);
	double plower = 0, pupper = 1;
	if (bb) {
	    if (isDiscreteValued() && jags_finite(lower)) {
		plower = p(lower - 1, parameters, true, false);
	    }
	    else {
		plower = p(lower, parameters, true, false);
	    }
	}
	if (ba) {
	    pupper = p(upper, parameters, true, false);
	}
	double pmed = (plower + pupper)/2;
	double med = q(pmed, parameters, true, false);	

	//Calculate the log densities
	double dlower = d(lower, parameters, true);
	double dupper = d(upper, parameters, true);
	double dmed = d(med, parameters, true);

	//Pick the median if it has the highest density, otherwise pick
	//a point near to (but not on) the boundary
	if (dmed > dlower && dmed > dupper) {
	    y = med;
	}
	else if (dupper > dlower) {
	    y = q(0.1 * plower + 0.9 * pupper, parameters, true, false);
	}
	else {
	    y = q(0.9 * plower + 0.1 * pupper, parameters, true, false);
	}
    }
    return y;
}

double 
DistScalarRmath::logLikelihood(double x,
			       vector<double const *> const &parameters) const
{
    bool lb = lowerBound(this, parameters);
    bool ub = upperBound(this, parameters);

    double loglik =  d(x, parameters, true);
    if (lb || ub) {
	double lower, upper;
	support(&lower, &upper, parameters);
	if (x < lower || x > upper) {
       	  return JAGS_NEGINF;
	}
	double plower = 0, pupper = 1;
	if (lb) {
	    if (isDiscreteValued()) {
       	        if (jags_finite(lower)) {
		    //Need to correct for discreteness
		    plower = p(lower - 1, parameters, true, false);
		}
	    }
	    else {
		plower = p(lower, parameters, true, false);
	    }
	}
	if (ub) {
	    pupper = p(upper, parameters, true, false); 
	}
	loglik -= log(pupper - plower);
    }
    return loglik;
}


double DistScalarRmath::randomSample(vector<double const *> const &parameters,
				     RNG *rng) const
{
    bool bb = lowerBound(this, parameters);
    bool ba = upperBound(this, parameters);
  
    if (ba || bb) {
	double lower, upper;
	support(&lower, &upper, parameters);
	double plower = 0, pupper = 1;
	if (bb) {
	    if (isDiscreteValued()) {
	        if (jags_finite(lower)) {
		    plower = p(lower - 1, parameters, true, false);
		}
	    }
	    else {
		plower = p(lower, parameters, true, false);
	    }
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
