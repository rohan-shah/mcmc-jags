#include <config.h>
#include "DNormMix.h"
#include <util/dim.h>
#include <util/nainf.h>

#include <cmath>

#include <JRmath.h>

using std::vector;

#define MU(par) (par[0])
#define TAU(par) (par[1])
#define PROB(par) (par[2])

namespace mix {

    DNormMix::DNormMix()
	: Distribution("dnormmix", 3, false, false)
    {}

    bool 
    DNormMix::checkParameterDim(vector<vector<unsigned int> > const &dims) const
    {
	if (dims[0][0] == 1)
	    return false; // Must be a mixture
   
	// Parameter dimensions must match (but they need not be vectors)
	return (dims[1] == dims[0]) && (dims[2] == dims[0]);
    }

    bool 
    DNormMix::checkParameterValue(vector<double const *> const &par,
				  vector<vector<unsigned int> > const &dims) 
	const
    {
	unsigned int Ncat = product(dims[0]);
	double sump = 0.0;
	for (unsigned int i = 0; i < Ncat; ++i) {
	    if (TAU(par)[i] <= 0)
		return false;
	    if (PROB(par)[i] <= 0) 
		return false;
	    sump += PROB(par)[i];
	}
	return (fabs(sump - 1) < 16 * DBL_EPSILON);
    }

    double DNormMix::logLikelihood(double const *x, unsigned int length,
				   vector<double const *> const &par,
				   vector<vector<unsigned int> > const &dims,
				   double const *lower, double const *upper) 
	const
    {
	unsigned int Ncat = product(dims[0]);
	double density = 0.0;
	for (unsigned int i = 0; i < Ncat; ++i) {
	    density += PROB(par)[i] * dnorm(*x, MU(par)[i], 
					    1/sqrt(TAU(par)[i]), 0);
	}
	return log(density);
    }

    void 
    DNormMix::randomSample(double *x, unsigned int length,
			   vector<double const *> const &par, 
			   vector<vector<unsigned int> > const &dims, 
			   double const *lower, double const *upper, RNG *rng) 
	const
    {
	double const *mu = MU(par);
	double const *tau = TAU(par);
	unsigned long Ncat = product(dims[0]);
    
	// Select mixture component (r)
	unsigned int r = Ncat - 1;
	double sump = 0;
	double p_rand = runif(0, 1, rng);
	for (unsigned int i = 0; i < Ncat - 1; ++i) {
	    sump += PROB(par)[i];
	    if (sump > p_rand) {
		r = i;
		break;
	    }
	}

	// Now sample from conditional distribution of component r
	double ans = rnorm(mu[r], 1/sqrt(tau[r]), rng);
	*x = ans;
    }

    void DNormMix::typicalValue(double *x, unsigned int length,
				vector<double const *> const &par,
				vector<std::vector<unsigned int> > const &dims,
				double const *lower, double const *upper) const
    {
	double const *mu = MU(par);
	unsigned int Ncat = product(dims[0]);

	unsigned int j = 0;
	for (unsigned int i = 1; i < Ncat; ++i) {
	    if (PROB(par)[i] > PROB(par)[j]) {
		j = i;
	    }
	} 
    
	*x = mu[j];
    }

    bool DNormMix::isSupportFixed(std::vector<bool> const &fixmask) const
    {
	return true;
    }

    void 
    DNormMix::support(double *lower, double *upper, unsigned int length,
		      vector<double const *> const &parameters,
		      vector<vector<unsigned int> > const &dims) const
    {
	*lower = JAGS_NEGINF;
	*upper = JAGS_POSINF;
    }

}
