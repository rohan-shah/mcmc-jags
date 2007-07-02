#include <config.h>
#include "DGenGamma.h"

#include <JRmath.h>

/* if x ~ dgamma(r, mu, beta) then x^beta ~ dgamma(r, mu^beta) */

using std::vector;

#define SHAPE(par) (*par[0])
#define SCALE(par) exp (- (*par[2]) * log(*par[1]))
#define POW(par) (*par[2])

//The rate if POW(par) = 1 and we have a gamma distribution
#define URATE(par) (*par[1])

static inline double transform(double x,  vector<double const*> const &par)
{
    // y <- x^beta 
    return exp(POW(par) * log(x));
}

static inline double UNtransform(double x, vector<double const*> const &par)
{
    // y <- x^(-beta)
    return exp(log(x) / POW(par));
}

DGenGamma::DGenGamma()
    : DistScalarRmath("dgamma", 3, DIST_POSITIVE, true, false)
{}

bool 
DGenGamma::checkParameterValue(vector<double const *> const &par) const
{
    return (SHAPE(par) > 0 && URATE(par) > 0 && POW(par) > 0);
}

double
DGenGamma::d(double x, vector<double const *> const &par, bool give_log) 
    const
{
    double log_jacobian = (POW(par) - 1)*log(x) + log(POW(par));
    x = transform(x, par);
    double d = dgamma(x, SHAPE(par), SCALE(par), give_log);
    if (give_log) {
	return log_jacobian + d;
    }
    else {
	return exp(log_jacobian) * d;
    }
}

double
DGenGamma::p(double q, vector<double const *> const &par, bool lower,
	  bool give_log) const
{
    q = transform(q, par);
    return pgamma(q, SHAPE(par), SCALE(par), lower, give_log);
}

double 
DGenGamma::q(double p, vector<double const *> const &par, bool lower, 
	  bool log_p) const
{
    double q = qgamma(p, SHAPE(par), SCALE(par), lower, log_p);
    return UNtransform(q, par);
}

double DGenGamma::r(vector<double const *> const &par, RNG *rng) const
{
    double x = rgamma(SHAPE(par), SCALE(par), rng);
    return UNtransform(x, par);
}
