#include <config.h>
#include "DGamma.h"

#include <JRmath.h>
#include <util/nainf.h>

using std::vector;

#define SHAPE(par) (*par[0])
#define SCALE(par) (1 / *par[1])
#define RATE(par) (*par[1])

namespace bugs {

DGamma::DGamma()
    : RScalarDist("dgamma", 2, DIST_POSITIVE)
{}

bool 
DGamma::checkParameterValue (vector<double const *> const &par) const
{
    return (SHAPE(par) > 0 && RATE(par) > 0);
}

double
DGamma::d(double x, PDFType type,
	  vector<double const *> const &par, bool give_log)  const
{
    if (type == PDF_PRIOR) {
	//Avoid expensive evaluation of gamma function
	if (x < 0) {
	    return give_log ? JAGS_NEGINF : 0;
	}
	else if (x == 0) {
	    //Density at zero defined by continuity
	    return xlog0(SHAPE(par) - 1, give_log);
	}
	else {
	    double y = (SHAPE(par) - 1) * log(x) - RATE(par) * x;
	    return give_log ? y : exp(y);
	}
    }
    else {
	return dgamma(x, SHAPE(par), SCALE(par), give_log);
    }
}

double
DGamma::p(double q, vector<double const *> const &par, bool lower,
	  bool give_log) const
{
    return pgamma(q, SHAPE(par), SCALE(par), lower, give_log);
}

double 
DGamma::q(double p, vector<double const *> const &par, bool lower, 
	  bool log_p) const
{
    return qgamma(p, SHAPE(par), SCALE(par), lower, log_p);
}

double DGamma::r(vector<double const *> const &par, RNG *rng) const
{
    return rgamma(SHAPE(par), SCALE(par), rng);
}

double DGamma::typicalValue(vector<double const *> const &par,
			    double const *lower, double const *upper) const
{
    if (lower == 0 && upper == 0) {
	return SHAPE(par) * SCALE(par);
    }
    else {
	return RScalarDist::typicalValue(par, lower, upper);
    }
}

double DGamma::KL(vector<double const *> const &par1,
		  vector<double const *> const &par2) const
{
    double b1 = SHAPE(par1), b2 = SHAPE(par2); 
    double r = RATE(par2) / RATE(par1);
    return (r - 1) * b1  - b2 * log(r)
	+ (b1 - b2) * digamma(b1) + lgammafn(b2) - lgammafn(b1);
}

}
