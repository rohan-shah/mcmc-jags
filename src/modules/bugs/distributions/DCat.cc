#include <config.h>
#include <rng/RNG.h>
#include <sarray/util.h>
#include "DCat.h"

#include <cfloat>
#include <algorithm>
#include <cmath>
#include <stdexcept>

#include <JRmath.h>

using std::min;
using std::max;
using std::vector;
using std::max_element;
using std::logic_error;

#define PROB(par) (par[0])
#define NCAT(dims) (dims[0][0])

DCat::DCat()
    : Distribution("dcat", 1, false, true) 
{}

bool DCat::checkParameterDim(vector<vector<unsigned int> > const &dims) const
{
    return isVector(dims[0]);
}

bool DCat::checkParameterValue(vector<double const *> const &par,
			       vector<vector<unsigned int> > const &dims) const
{
    double sump = 0.0;
    for (unsigned int i = 0; i < NCAT(dims); i++) {
	if (PROB(par)[i] < 0.0 || PROB(par)[i] > 1.0) {
	    return false; // Invalid probability
	}
	sump += PROB(par)[i];
    }
    if (fabs(sump - 1.0) > 16 * DBL_EPSILON)
	return false; // Probabilities do not sum to 1 

    return true;
}

double DCat::logLikelihood(double const *x, unsigned int length,
		     vector<double const *> const &par,
		     vector<vector<unsigned int> > const &dims) const
{
    unsigned int y = static_cast<unsigned int>(*x);
    if (y >= 1 && y <= NCAT(dims)) {
	return log(PROB(par)[y-1]);
    }
    else {
        return -DBL_MAX;
    }
}

void DCat::randomSample(double *x, unsigned int length,
		  std::vector<double const *> const &par,
		  std::vector<std::vector<unsigned int> > const &dims,
		  RNG *rng) const
{
    double p = rng->uniform();
    double sump = 0;
    for (unsigned int i = 1; i < NCAT(dims); ++i) {
	sump += PROB(par)[i-1];
	if (sump > p + 16 * DBL_EPSILON) {
	    *x = i;
	    return;
	}
    }
    *x  = NCAT(dims);
}

std::vector<unsigned int> 
DCat::dim(std::vector<std::vector<unsigned int> > const &dims) const
{
    return vector<unsigned int>(1,1);
}

void DCat::support(double *lower, double *upper, unsigned int length,
	           vector<double const *> const &par,
	           vector<vector<unsigned int> > const &dims) const
{
    if (length != 1)
	throw logic_error("Invalid length in DCat::support");

    *lower = 1;
    *upper = dims[0][0];
}

void DCat::typicalValue(double *x, unsigned int length,
		  std::vector<double const *> const &par,
		  std::vector<std::vector<unsigned int> > const &dims) const
{
    //Pick the mode
    *x = max_element(PROB(par), PROB(par) + NCAT(dims)) - PROB(par) + 1;
}


/*
double DCat::l(vector<double const *> const &par) const
{
   return 1;
}

double DCat::u(vector<double const *> const &par) const
{
   return par[0]->length();
}

double DCat::d(double x, std::vector<double const *> const &par, 
	       bool give_log) const
{
  double const *prob = par[0]->value();
  long length = par[0]->length();
  double value = 0;

  long y = static_cast<long>(x);
  if (y >= 1 && y <= length) {
    value =  prob[y-1];
  }

  if (give_log) {
    return value == 0 ? -DBL_MAX : log(value);
  }
  else {
    return value;
  }
}

double DCat::p(double x, std::vector<double const *> const &par,
               bool lower, bool give_log) const
{
  double const *prob = par[0]->value();

  double sump = 0;  
  if (x > par[0]->length()) {
    sump = 1;
  }
  else {
    for (long i = 1; i <= x; ++i) {
      sump += prob[i-1];
    }
  }

  if (!lower) {
    sump = max(1 - sump, 0.0);
  }

  if (give_log) {
    return sump == 0 ? -DBL_MAX : log(sump);
  }
  else {
    return sump;
  }
}

double DCat::q(double p, std::vector<double const *> const &par,
	     bool lower, bool log_p) const
{
  if (log_p)
    p = exp(p);
  if (!lower)
    p = 1 - p;

  long length = par[0]->length();
  double const *prob = par[0]->value();

  double sump = 0;
  for (long i = 1; i < length; ++i) {
    sump += prob[i-1];
    if (sump > p + 16*DBL_EPSILON)
      return i;
  }
  return length;
}

double DCat::r(std::vector<double const *> const &par, RNG *rng) const
{
  return q(rng->uniform(), par, true, false);
}
*/

bool DCat::isSupportFixed(std::vector<bool> const &fixmask) const
{
    return true;
}
