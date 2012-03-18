#include <config.h>
#include "KS.h"
#include <rng/RNG.h>
#include <cmath>

#define PI            3.141592653589793238462643383280 
#define PI_SQUARE     9.86960440108936

using std::log;
using std::exp;
using std::pow;
using std::sqrt;
using std::fabs;

static bool r_intvl(double u, double lambda)
{
    // Rejection algorithm based on an alternating series expansion
    // of the KS distribution. This is valid for lambda > 4/3

    if (u==0) {
	return false;
    }

    double z = 1;
    double x = exp(-lambda/2);
    int j = 0;
    while(true) {
	j++;
	int j2 = (j+1)*(j+1);
	z -= j2 * pow(x, j2 - 1);
	if (z > u)
	    return true;
	j++;
	j2 = (j+1)*(j+1);
	z += j2 * pow(x, j2 - 1);
	if (z < u)
	    return false;
    }
}

static bool l_intvl(double u, double lambda)
{
    // Rejection algorithm based on an alternating series expansion
    // of the KS distribution. This is valid for lambda < pi^2

    if (u==0) {
	return false; // Avoid numerical problems with log(u)
    }
    if (lambda < 1E-3) {
	return false; //Acceptance probability -> 0 in this range
    }

    double h = 0.5*log(2.0) + 2.5*log(PI) - 2.5*log(lambda) - 
	PI_SQUARE/(2 * lambda) + 0.5*lambda;
    double logu = log(u);
    double z = 1;
    double x = exp(-PI_SQUARE/(2*lambda));
    double k = lambda / PI_SQUARE;
    int j = 0;
    while(true) {
	j++;
	z -= k*pow(x, j*j-1);
	if (h + log(z) > logu)
	    return true;
	j++;
	int j2 = (j+1)*(j+1);
	z += j2*pow(x,j2-1);
	if (h + log(z) < logu)
	    return false;
    }
}

static bool accept(double lambda, RNG *rng)
{
    // Accepts or rejects proposal lambda generated from a GIG
    // distribution.  The rejection algorithm is proposed by Holmes
    // and Held (2006) and adapted from the original algorithm for
    // sampling from the KS distribution by Devroye (1986).

    // We use separate alternating series expansions for lambda <=
    // 3.1039 and lambda > 3.1039.  This cut-off was chosen after
    // numerical investigation of the series expansions to optimize
    // convergence.  With this cutoff, the error is no more than
    // 3.6E-5 when the expansion is truncated to the first
    // term. Hence, although both l_intvl and r_intvl contain infinite
    // loops, they will exit almost immediately.

    double u = rng->uniform();
    return lambda > 3.1039 ? r_intvl(u, lambda) : l_intvl(u, lambda);
}


static double gig(double delta, RNG *rng)
{
    // Draws a sample from the generalized inverse gaussian distribution
    // GIG(0.5, 1, delta^2) = delta/IG(1, delta). 
    //
    // The IG (Inverse Gaussian) distribution is sampled using the
    // algorithm of Michael, Schucany and Haas (1976) as described by
    // Devroye (1986).
    //
    // Note that as delta -> 0, GIG(0.5, 1, delta^2) tends to a
    // chisquare distribution on 1 df. We use this approximation for
    // small delta.
    
    double y = rng->normal();
    y = y * y;
    if (delta <= 1.0E-6 * y) {
	// Inversion is numerically unstable for delta/y < 1.E-8
	return y;
    }
    else {
	y = 1 + (y - sqrt(y * (4*delta + y))) / (2*delta);
	return (rng->uniform() <= 1/(1 + y)) ? delta/y : delta*y;
    }
}

namespace glm {

    double sample_lambda(double delta, RNG *rng)
    {
	delta = fabs(delta);
	while (true) {
	    //Acceptance probability is at least 0.24 for all delta
	    double lambda = gig(delta, rng);
	    if (accept(lambda, rng)) return lambda;
	}
    }

}
	
