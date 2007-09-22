#include <config.h>
#include "DNormMix.h"
#include <sarray/util.h>
#include <sarray/nainf.h>

#include <cmath>

#include <JRmath.h>

using std::vector;

#define MU(par) (par[0])
#define TAU(par) (par[1])
#define PROB(par) (par[2])

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
			      vector<vector<unsigned int> > const &dims) const
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
			       double const *lower, double const *upper) const
{
    unsigned int Ncat = product(dims[0]);
    double density = 0.0;
    for (unsigned int i = 0; i < Ncat; ++i) {
	density += PROB(par)[i] * dnorm(*x, MU(par)[i], 1/sqrt(TAU(par)[i]), 0);
    }
    return log(density);
}

/*
DNormMix::p(double q, vector<double const *> const &par, bool lower, bool give_log)
    const
{
    double const *mu = MU(par);
    double const *tau = TAU(par);
    double const *prob = PROB(par);
    unsigned long Ncat = NCAT(par);
    
    double density = 0.0;
    for (unsigned int i = 0; i < Ncat; ++i) {
	density += prob[i] * pnorm(q, mu[i], 1/sqrt(tau[i]), lower, 0);
    }
    return give_log ? log(density) : density;
}

static double 
pmix(double q, double const *mu, double const *sigma, double const *p,
     unsigned int Ncat) 
{
    // Probability distribution function of a normal mixture

    double density = 0.0;
    for (unsigned int i = 0; i < Ncat; ++i) {
	density += p[i] * pnorm(q, mu[i], sigma[i], true, false);
    }
    return density;
}

static double 
qmix (double p, double const *mu, double const *sigma,
      double const *prob, unsigned int Ncat)
{
    // Calculate quantile of normal mixture by bisection
    // Based on an algorithm in the R package ensembleBMA by
    // Adrian E. Raftery, J. McLean Sloughter, and Michael Polakowski, 

    const double C = qnorm(p, 0, 1, true, false);
    double lower = mu[0] - C * sigma[0];
    double upper = mu[0] + C * sigma[0];
    double mid = (lower + upper)/2;

    while (upper - lower > 16 * sqrt(DBL_EPSILON)) {
	if (pmix(mid, mu, sigma, prob, Ncat) > p) {
	    upper = mid;
        }
        else {
            lower = mid;
        }
        mid = (lower + upper)/2;
    }
    return mid;
}

double 
DNormMix::q(double p, vector<double const *> const &par, bool lower, bool log_p)
    const
{
    if (!lower)
	p = 1 - p;
    if (log_p)
	p = exp(p);

    double const *mu = MU(par);
    double const *tau = TAU(par);
    double const *prob = PROB(par);
    unsigned long Ncat = par[0]->length();
    
    double * sigma = new double[Ncat];
    for (unsigned int i = 0; i < Ncat; ++i) {
	sigma[i] = 1/sqrt(tau[i]);
    }
    double q = qmix(p, mu, sigma, prob, Ncat);
    delete [] sigma;

    return q;
}
*/

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

