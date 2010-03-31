/* 
   Robert CP (1995) Simulation of truncated normal variables.
   Statistics and computing 5: 121-125.
*/
#include <config.h>

#include <rng/TruncatedNormal.h>
#include <rng/RNG.h>
#include <util/nainf.h>

#include <cmath>
#include <stdexcept>

using std::sqrt;
using std::exp;
using std::logic_error;

//sqrt(2*pi)
#define STP 2.506628274631

//Calculates optimal scale parameter for exponential envelope
static double Alpha(double mu)
{
    return (mu + sqrt(mu*mu + 4))/2;
}

static double lnorm(double left, RNG *rng)
{
    if (!jags_finite(left)) {
	throw logic_error("Non-finite boundary in truncated normal");
    }
    if (left < 0) {
	//Repeated sampling until truncation satisfied
	while(true) {
	    double y = rng->normal();
	    if (y >= left)
		return y;
	}
    }
    else {
	//Rejection sampling with exponential envelope
	double alpha = Alpha(left);
	while (true) {
	    double y = rng->exponential();
	    double C = (1 - y)/alpha;
	    if (rng->uniform() <= exp(-C*C/2))
		return left + y/alpha;
	    /*
            //Original version, less stable for large mu
	    double z = left + rng->exponential() / alpha;
	    if (rng->uniform() <= exp(-(alpha - z)*(alpha - z)/2))
		return z;
	    */
	}
    }
}

static double rnorm(double right, RNG *rng)
{
    return -lnorm(-right, rng);
}

/* 
   Rejection sampling of interval-truncated standard normal
   distribution using a uniform envelope.  This works best when we are
   sampling a short interval.x  
*/
static double inorm_unif(double left, double right, RNG *rng)
{
    double zmax; //Value of z at which density is maximum
    if (left > 0)
	zmax = left;
    else if (right < 0)
	zmax = right;
    else 
	zmax = 0;

    while (true) {
	double z = left + (right - left) * rng->uniform();
	if (rng->uniform() <= exp((zmax*zmax - z*z)/2)) {
	    return z;
	}
    }
}

/*
  Sampling from a interval-truncated standard normal by repeated draws
  from a non-truncated standard normal distribution. We keep sampling
  until the sampled value lies in the required interval.  This works
  best when we are sampling a wide interval, and are not sampling from
  one of the tails (so that left < 0, right > 0).  
*/
static double inorm_repeat(double left, double right, RNG *rng)
{
    double y = rng->normal();
    while (y < left || y > right) {
	y = rng->normal();
    }
    return y;
}

/*
  Sample an interval-truncated normal distribution when the
  interval [left,right] is in the right tail of the underlying normal
  distribution (so left > 0).
  
  If the interval is short, we do this by rejection
  sampling. Otherwise we do it by repeated sampling from a left
  truncated normal until the right limit is also satisfied.
*/
static double inorm_right_tail(double left, double right, RNG *rng)
{
    double alpha = Alpha(left);
    double maxdiff = exp((left*left - alpha*left + 1)/2)/alpha;
    if (right - left <= maxdiff) {
	//Rejection sampling with uniform envelope
	return inorm_unif(left, right, rng);
    }
    else {
	//Interval too wide for rejection sampling. Do repeat sampling
	//from left-truncated normal
	double y = lnorm(left, rng);
	while (y >= right) {
	    y = lnorm(left, rng);
	}
	return y;
    }
}


static double inorm(double left, double right, RNG *rng)
{
    if (!jags_finite(left) || !jags_finite(right)) {
	throw logic_error("Non-finite boundary in truncated normal");
    }
    if (right < left) {
	throw logic_error("Invalid limits in inorm");
    }

    if (left > 0) {
	return inorm_right_tail(left, right, rng);
    }
    else if (right < 0) {
	return -inorm_right_tail(-right, -left, rng);
    }
    else if (right - left < STP) {
	return inorm_unif(left, right, rng);
    }
    else {
	return inorm_repeat(left, right, rng);
    }
}

/* Public interface */

double lnormal(double left, RNG *rng, double mu, double sigma) {
    return mu + sigma * lnorm((left - mu)/sigma, rng);
}

double rnormal(double right, RNG *rng, double mu, double sigma) {
    return mu + sigma * rnorm((right - mu)/sigma, rng);
}

double inormal(double left, double right, RNG *rng, double mu, double sigma) {
    return mu + sigma * inorm((left - mu)/sigma, (right - mu)/sigma, rng);
}
