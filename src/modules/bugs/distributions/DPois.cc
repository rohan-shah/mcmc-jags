#include <config.h>
#include "DPois.h"
#include <util/nainf.h>

#include <limits.h>
#include <algorithm>

#include <JRmath.h>

using std::vector;
using std::max;

#define LAMBDA(par) (*par[0])
#define R_D_nonint(x)     (fabs((x) - floor((x)+0.5)) > 1e-7)

namespace bugs {

DPois::DPois()
    : RScalarDist("dpois", 1, DIST_POSITIVE, true)
{}

bool DPois::checkParameterValue (vector<double const *> const &par) const
{
    return (LAMBDA(par) >= 0);
}

double
DPois::d(double x, PDFType type,
	 vector<double const *> const &par, bool give_log) const
{
    if (type == PDF_LIKELIHOOD) {
	//Avoid expensive normalizing constant
	double lambda = LAMBDA(par);
	if (x < 0 || (lambda == 0 && x != 0) || R_D_nonint(x) || 
	    !jags_finite(lambda)) 
	{
	    return give_log ?  JAGS_NEGINF : 0;
	}
	double y = -lambda;
	if (lambda > 0) {
	    y += x * log(lambda);
	}
	return give_log ? y : exp(y);
    }
    else {
	return dpois(x, LAMBDA(par), give_log);
    }
}

double
DPois::p(double q, vector<double const *> const &par, bool lower, bool give_log)
    const
{
    return ppois(q, LAMBDA(par), lower, give_log);
}

double 
DPois::q(double p, vector<double const *> const &par, bool lower, bool log_p)
    const
{
    return qpois(p, LAMBDA(par), lower, log_p);
}

double DPois::r(vector<double const *> const &par, RNG *rng) const
{
    return rpois(LAMBDA(par), rng);
}

double DPois::KL(vector<double const *> const &par1,
		 vector<double const *> const &par2) const
{
    double lambda1 = LAMBDA(par1);
    double lambda2 = LAMBDA(par2);

    return lambda1 * (log(lambda1) - log(lambda2)) - lambda1 + lambda2;
}

}
