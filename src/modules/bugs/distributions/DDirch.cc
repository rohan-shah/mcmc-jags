#include <config.h>
#include "DDirch.h"
#include <rng/RNG.h>
#include <sarray/util.h>
#include <sarray/nainf.h>

#include <stdexcept>
#include <cmath>

#include <JRmath.h>

using std::vector;
using std::length_error;
using std::logic_error;

#define ALPHA(par) (par[0])
#define LENGTH(dims) (dims[0][0])


/* The Dirichlet distribution is extended to allow zero shape parameters.
   These represent structural zeros: when x ~ ddirch(alpha) is forward
   sampled, x[i] = 0 when alpha[i] = 0. To avoid trapping states in the
   model, structural zeros are only allowed when the alpha parameter
   is fixed.
*/

DDirch::DDirch()
  : Distribution("ddirch", 1, false, false) 
{}

vector<unsigned int> DDirch::dim(vector<vector<unsigned int> > const &dims) const
{
    return dims[0];
}

bool DDirch::checkParameterDim(vector<vector<unsigned int> > const &dims) const
{
    return isVector(dims[0]);
}

bool
DDirch::checkParameterValue(vector<double const *> const &par,
                            vector<vector<unsigned int> > const &dims) const
{
    double const *alpha = ALPHA(par);
    unsigned int length = LENGTH(dims);

    for (unsigned int i = 0; i < length; i++) {
	if (alpha[i] <= 0) {
	    return false;
	}
    }

    /*
    unsigned int nzero = 0; // No of zero shape par
    for (unsigned int i = 0; i < length; i++) {
	if (alpha[i] <= 0) {
	    if (alpha[i] == 0) {
		nzero++;
	    }
	    else {
		return false;
	    }
	}
    }

    if (nzero) {
	return (nzero < length);
    }
    else {
	return true;
    }
    */
}

double DDirch::logLikelihood(double const *x, unsigned int length,
			     vector<double const *> const &par,
                             vector<vector<unsigned int> > const &dims) const
{
    double const *alpha = ALPHA(par);

    double alphasum = 0.0;
    double loglik = 0.0;
    for (unsigned int i = 0; i < length; i++) {
	if (alpha[i] == 0) {
	    if (x[i] > 0)
		return JAGS_NEGINF;
	}
	else {
	    alphasum += alpha[i];
	    loglik += (alpha[i] - 1) * x[i] - lgamma(alpha[i]);
	}
    }
    loglik += lgamma(alphasum);

    return loglik;
}

void DDirch::randomSample(double *x, unsigned int length,
                          vector<double const *> const &par,
                          vector<vector<unsigned int> > const &dims,
			  RNG *rng) const
{
    double const *alpha = ALPHA(par);

    /* Generate independent gamma random variables, then normalize
       to create Dirichlet distribution.
    */
    double sumx = 0.0;
    for (unsigned int i = 0; i < length; i++) {
	x[i] = (alpha[i]==0) ? 0 : rgamma(alpha[i], 1, rng);
	sumx += x[i];
    }
    for (unsigned int j = 0; j < length; j++) {
	x[j] /= sumx;
    }
}

void DDirch::support(double *lower, double *upper, unsigned int length,
		vector<double const *> const &par,
		vector<vector<unsigned int> > const &dims) const
{
   for (unsigned int i = 0; i < length; ++i) {
      lower[i] = 0;
      if (ALPHA(par)[i] == 0)
         upper[i] = 0;
      else
         upper[i] = JAGS_POSINF;
   }
}

void DDirch::typicalValue(double *x, unsigned int length,
                          vector<double const *> const &par,
                          vector<vector<unsigned int> > const &dims) const
{
  double alphasum = 0.0;
  for (unsigned int i = 0; i < length; ++i) {
    alphasum += ALPHA(par)[i];
  }
  for (unsigned int i = 0; i < length; ++i) {
    x[i] = ALPHA(par)[i]/alphasum;
  }
}

bool DDirch::isSupportFixed(vector<bool> const &fixmask) const
{
    return fixmask[0];
}

unsigned int DDirch::df(vector<vector<unsigned int> > const &dims) const
{
    return dims[0][0] - 1;
}
