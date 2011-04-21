#include <config.h>
#include "DChisqr.h"

#include <JRmath.h>

using std::vector;
using std::string;

#define DF(par) (*par[0])

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
DChisqr::d(double x, vector<double const *> const &par, bool give_log) const
{
    return dchisq(x, DF(par), give_log);
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
