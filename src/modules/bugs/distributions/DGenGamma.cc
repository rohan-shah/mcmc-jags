#include <config.h>
#include "DGenGamma.h"

#include <JRmath.h>

/* if x ~ dgamma(r, mu, beta) then (mu*x)^beta ~ dgamma(r, 1) */

using std::vector;
using std::string;

#define SHAPE(par) (*par[0])
//The rate if POW(par) = 1 and we have a gamma distribution
#define URATE(par) (*par[1])
#define POW(par) (*par[2])

static inline double transform(double x,  vector<double const*> const &par)
{
    // y <- (mu*x)^beta 
    return exp(POW(par) * (log(x) + log(URATE(par))));
}

static inline double UNtransform(double x, vector<double const*> const &par)
{
    // x <- y^(1/beta) / mu
    return exp(log(x) / POW(par) - log(URATE(par)));
}

namespace bugs {

DGenGamma::DGenGamma()
    : RScalarDist("dgen.gamma", 3, DIST_POSITIVE)
{}

string DGenGamma::alias() const
{
    return "gengamma";
}

bool DGenGamma::checkParameterValue (vector<double const *> const &par) const
{
    return (SHAPE(par) > 0 && URATE(par) > 0 && POW(par) > 0);
}

double
DGenGamma::d(double x, PDFType type,
vector<double const *> const &par, bool give_log) 
    const
{
    double log_jacobian = (POW(par) - 1)*log(x) + log(POW(par)) 
	+ POW(par) * log(URATE(par));
    x = transform(x, par);
    double d = dgamma(x, SHAPE(par), 1.0, give_log);
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
    return pgamma(q, SHAPE(par), 1.0, lower, give_log);
}

double 
DGenGamma::q(double p, vector<double const *> const &par, bool lower, 
	  bool log_p) const
{
    double q = qgamma(p, SHAPE(par), 1.0, lower, log_p);
    return UNtransform(q, par);
}

double DGenGamma::r(vector<double const *> const &par, RNG *rng) const
{
    double x = rgamma(SHAPE(par), 1.0, rng);
    return UNtransform(x, par);
}

double DGenGamma::KL(vector<double const *> const &par1,
		     vector<double const *> const &par2) const
{
    // Collapses to gamma distribution with beta1 = beta2 = 1;

    double beta1 = POW(par1), beta2 = POW(par2);
    double theta = URATE(par2) / URATE(par1); 
    double phi = beta2 / beta1;
    double r1 = SHAPE(par1), r2 = SHAPE(par2);

    return - log(phi) - beta2 * r2 * log(theta)
	+ (r1 -  phi * r2) * digamma(r1)
	+ pow(theta, beta2) * gammafn(r1 + phi) / gammafn(r1) - r1
	+ lgammafn(r2) - lgammafn(r1);
}

}
