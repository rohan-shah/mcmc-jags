/* 
   Robert CP (1995) Simulation of truncated normal variables.
   Statistics and computing 5: 121-125.
*/
#include <config.h>

#include "TruncatedNormal.h"

#include <rng/RNG.h>
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

/*
  Left truncated standard normal
*/
static double lnorm(double left, RNG *rng)
{
    if (left < 0) {
	//Repeated sampling until truncation satisfied
	double y = rng->normal();
	while(y < left) {
	    y = rng->normal();
	}
	return y;
    }
    else {
	//Rejection sampling with exponential envelope
	double alpha = Alpha(left);
	double z = left + rng->exponential() / alpha;
	while (rng->uniform() < exp(-(alpha - z)*(alpha - z)/2)) {
	    z = left + rng->exponential() / alpha;
	}
	return z;
    }
}

/* 
   Rejection sampling of interval-truncated standard normal
   distribution using a uniform envelope.  This works best when we are
   sampling a short interval.x  
*/
static double inorm_unif(double left, double right, RNG *rng)
{
    //Value of z at which density reaches maximum value
    double zmax;
    if (left > 0)
	zmax = left;
    else if (right < 0)
	zmax = right;
    else 
	zmax = 0;

    while (true) {
	double z = left + (right - left) * rng->uniform();
	if (rng->uniform() < exp((zmax*zmax - z*z)/2)) {
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
static bool inorm_right_tail(double left, double right, RNG *rng)
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

namespace glm {

    double LNorm(double mu, double sigma, double left, RNG *rng) {
	return mu + sigma * lnorm((left - mu)/sigma, rng);
    }

    double RNorm(double mu, double sigma, double right, RNG *rng) {
	return mu - sigma * lnorm((mu - right)/sigma, rng);
    }

    double INorm(double mu, double sigma, double left, double right, RNG *rng) {
	return mu + sigma * inorm((left - mu)/sigma, (right - mu)/sigma, rng);
    }

}

    
