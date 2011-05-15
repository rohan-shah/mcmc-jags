#include <config.h>
#include "DChisqr.h"

#include <JRmath.h>
#include <util/nainf.h>

using std::vector;
using std::string;

#define DF(par) (*par[0])

namespace bugs {

DChisqr::DChisqr()
    : RScalarDist("dchisqr", 1, DIST_POSITIVE)
{}

string DChisqr::alias() const
{
    return "dchisq";
}

bool 
DChisqr::checkParameterValue (vector<double const *> const &par) const
{
    return (DF(par) > 0);
}

double 
DChisqr::d(double x, PDFType type,
	   vector<double const *> const &par, bool give_log) const
{
    if (type == PDF_PRIOR) {
	//Avoid expensive evaluation of gamma function
	if (x < 0) {
	    return give_log ? JAGS_NEGINF : 0;
	}
	else if (x == 0) {
	    //Density at zero defined by continuity
	    return xlog0(DF(par) - 2, give_log);
	}
	else {
	    double y = (DF(par)/2 - 1) * log(x) - x/2;
	    return give_log ? y : exp(y);
	}
    }
    else {
	return dchisq(x, DF(par), give_log);
    }
}

double 
DChisqr::p(double q, vector<double const *> const &par, bool lower, bool log_p)
  const
{
    return pchisq(q, DF(par), lower, log_p);
}

double
DChisqr::q(double p, vector<double const *> const &par, bool lower, bool log_p)
const
{
    return qchisq(p, DF(par), lower, log_p);
}

double DChisqr::r(vector<double const *> const &par, RNG *rng) const
{
    return rchisq(DF(par), rng);
}

double DChisqr::KL(vector<double const *> const &par1,
		   vector<double const *> const &par2) const
{
    //Specialisation of the gamma Kullback-Leibler divergence
    double b1 = DF(par1)/2, b2 = DF(par2)/2; 
    return (b1 - b2) * digamma(b1) + lgammafn(b2) - lgammafn(b1);
}

}
