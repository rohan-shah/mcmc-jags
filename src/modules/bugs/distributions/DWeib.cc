#include <config.h>
#include "DWeib.h"

#include <rng/RNG.h>

#include <vector>
#include <cmath>

#include <JRmath.h>

using std::vector;

#define SHAPE(par) (*par[0])
#define SCALE(par) (pow(*par[1], -1/(*par[0])))

#define V(par) (*par[0])
#define LAMBDA(par) (*par[1])

DWeib::DWeib()
    : RScalarDist("dweib", 2, DIST_POSITIVE)
{}

bool 
DWeib::checkParameterValue (vector<double const *> const &par) const
{
    return (*par[0] > 0 && *par[1] > 0);
}

double 
DWeib::d(double x, vector<double const *> const &par, bool give_log)
    const
{
/*
    double v = V(par);
    double lambda = LAMBDA(par);
	
    if (give_log) {
	return -lambda * pow(x, v) + log(v) + log(lambda) + (v - 1) * log(x);
    }
    else {
	return v * lambda * pow(x, v-1) * exp(-lambda*pow(x,v));
    }
*/
    return dweibull(x, SHAPE(par), SCALE(par), give_log);
}

double
DWeib::p(double q, vector<double const *> const &par, bool lower, bool give_log)
  const
{
/*
    double lambda = LAMBDA(par);
    double v = V(par);

    double log_p = -lambda * pow(q, v);
    if (lower) {
        return give_log ? log_p : exp(log_p);
    }
    else {
	double p = 1 - exp(log_p);
        return give_log ? log(p) : p;
    }
*/
	
    return pweibull(q, SHAPE(par), SCALE(par), lower, give_log);
}

double 
DWeib::q(double p, vector<double const *> const &par, bool lower, bool log_p)
    const
{
/*
    if (lower) {
	if (!log_p)
	    p = log(p);
    }
    else {
	if (log_p) {
	    p = exp(p);
	}
	p = log1p(-p);
    }

    return exp(log(-p/LAMBDA(par))/V(par));
*/

    return qweibull(p, SHAPE(par), SCALE(par), lower, log_p);
}

double DWeib::r(vector<double const *> const &par, RNG *rng) const
{
    //return exp(log(rng->exponential()/LAMBDA(par))/V(par));

    return rweibull(SHAPE(par), SCALE(par), rng);
}
