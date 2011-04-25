#include <config.h>
#include "DBeta.h"

#include <JRmath.h>

using std::vector;

namespace bugs {

DBeta::DBeta()
    : RScalarDist("dbeta", 2, DIST_PROPORTION)
{}

bool 
DBeta::checkParameterValue (vector<double const *> const &par) const
{
    return (*par[0] > 0.0 && *par[1] > 0.0);
}

double 
DBeta::d(double x, vector<double const *> const &par, bool give_log) const
{
    return dbeta(x, *par[0], *par[1], give_log);
}

double 
DBeta::p(double q, vector<double const *> const &par, bool lower, bool log_p) 
  const
{
    return pbeta(q, *par[0], *par[1], lower, log_p);
}

double 
DBeta::q(double p, vector<double const *> const &par, bool lower, bool log_p) 
  const
{
    return qbeta(p, *par[0], *par[1], lower, log_p);
}

double DBeta::r(vector<double const *> const &par, RNG *rng) const
{
    return rbeta(*par[0], *par[1], rng);
}

double DBeta::KL(vector<double const *> const &par1, 
		 vector<double const *> const &par2) const
{
    double a1 = *par1[0];
    double b1 = *par1[1];
    double a2 = *par2[0];
    double b2 = *par2[1];

    return lbeta(a2, b2) - lbeta(a1, b1) 
	- (a2 - a1) * digamma(a1)
	- (b2 - b1) * digamma(b1)
	+ (a2 + b2 - a1 - b1) * digamma(a1 + b1);
}

}
