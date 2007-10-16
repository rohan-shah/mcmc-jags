#include <config.h>
#include "DMulti.h"
#include <rng/RNG.h>
#include <util/dim.h>
#include <util/nainf.h>

#include <stdexcept>
#include <cmath>

#include <JRmath.h>

using std::vector;
using std::length_error;
using std::logic_error;

#define PROB(par) (par[0])
#define SIZE(par) (*par[1])

DMulti::DMulti()
  : Distribution("dmulti", 2, false, true) 
{}

bool DMulti::checkParameterDim(vector<vector<unsigned int> > const &dims) const
{
  //Check that PROB is a vector and SIZE is a scalar
  return isVector(dims[0]) && isScalar(dims[1]);
}

bool DMulti::checkParameterDiscrete(vector<bool> const &mask) const
{
    return mask[1]; //SIZE is discrete-valued
}

bool 
DMulti::checkParameterValue(vector<double const *> const &par,
			    vector<vector<unsigned int> > const &dims) const
{
    if (SIZE(par) < 1)
	return false;

    double const *prob = PROB(par);
    unsigned int length = product(dims[0]);
    for (unsigned int i = 0; i < length; i++) {
      if (prob[i] < 0)
	return false;
    }

    return true;
}

double DMulti::logLikelihood(double const *x, unsigned int length,
			     vector<double const *> const &par,
			     vector<vector<unsigned int> > const &dims,
			     double const *lower, double const *upper) const
{
    double const *prob = PROB(par);

    double loglik = 0.0;
    double xsum = 0.0;
    double sump = 0.0;
    for (unsigned int i = 0; i < length; i++) {
	if (prob[i] == 0) {
	    if (x[i] != 0)
		return JAGS_NEGINF;
	}
	else if (x[i] != 0) {
	     loglik += x[i] * log(prob[i]) - lgamma(x[i] + 1);
	     xsum += x[i];
	}
	sump += prob[i];
    }
    loglik += lgamma(xsum + 1) - xsum * log(sump);

    return loglik;
}

void DMulti::randomSample(double *x, unsigned int length,
			  vector<double const *> const &par,
			  vector<vector<unsigned int> > const &dims,
			  double const *lower, double const *upper,
			  RNG *rng) const
{
    /* Sample multinomial as a series of binomial distributions */
    double N = SIZE(par);
    double sump = 1.0;
    double const *prob = PROB(par);
    for (unsigned int i = 0; i < length - 1; i++) {
	if (N == 0) {
	    x[i] = 0;
	}
	else {
	    x[i] = rbinom(N, prob[i]/sump, rng);
	    N -= x[i];
	    sump -= prob[i];
	}
    }
    x[length - 1] = N;
}

void DMulti::support(double *lower, double *upper, unsigned int length,
	     vector<double const *> const &par,
	     vector<vector<unsigned int> > const &dims) const
{
    for (unsigned int i = 0; i < length; ++i) {
	lower[i] = 0;
        if (PROB(par)[i] == 0) 
           upper[i] = 0;
        else
	   upper[i] = SIZE(par);
    }
}

vector<unsigned int> 
DMulti::dim(vector<vector<unsigned int> > const &dims) const
{
    return dims[0];
}


void DMulti::typicalValue(double *x, unsigned int length,
			  vector<double const *> const &par,
			  vector<vector<unsigned int> > const &dims,
			  double const *lower, double const *upper) const
{
    /* Draw a typical value in the same way as a random sample, but
       substituting the median at each stage */

    double N = SIZE(par);
    double sump = 1.0;
    double const *prob = PROB(par);
    for (unsigned int i = 0; i < length - 1; i++) {
	if (N == 0) {
	    x[i] = 0;
	}
	else {
	    x[i] = qbinom(0.5, N, prob[i]/sump, true, false);
	    N -= x[i];
	    sump -= prob[i];
	}
    }
    x[length - 1] = N;
}

bool DMulti::isSupportFixed(std::vector<bool> const &fixmask) const
{
    return fixmask[1];
}

unsigned int DMulti::df(vector<vector<unsigned int> > const &dims) const
{
    return dims[0][0] - 1;
} 
