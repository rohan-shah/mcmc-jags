#include <config.h>
#include "DBin.h"

#include <algorithm>

#include <JRmath.h>

using std::vector;
using std::max;
using std::min;
using std::string;

/* BUGS parameterization is in opposite order to R parameterization */
#define SIZE(par) (*par[1])
#define PROB(par) (*par[0])

DBin::DBin()
    : RScalarDist("dbin", 2, DIST_SPECIAL, true)
{}

string DBin::alias() const
{
    return "dbinom";
}

bool 
DBin::checkParameterDiscrete (vector<bool> const &mask) const
{
    return mask[1] == true;
}

bool DBin::checkParameterValue (vector<double const *> const &par) const
{
    return (SIZE(par) >= 0 && PROB(par) >= 0.0 && PROB(par) <= 1.0);
}

double DBin::d(double x, vector<double const *> const &par, 
	       bool give_log) const
{
    return dbinom(x, SIZE(par), PROB(par), give_log);
}

double DBin::p(double x, vector<double const *> const &par, 
	       bool lower, bool give_log) const
{
    return pbinom(x, SIZE(par), PROB(par), lower, give_log);
}

double DBin::q(double p, vector<double const *> const &par, 
	       bool lower, bool log_p) const
{
    return qbinom(p, SIZE(par), PROB(par), lower, log_p);
}

double DBin::r(vector<double const *> const &par, RNG *rng) const
{
    return rbinom(SIZE(par), PROB(par), rng);
}

double 
DBin::l(vector<double const *> const &par) const
{
    return 0;
}
	

double 
DBin::u(vector<double const *> const &par) const
{
    return SIZE(par);
}

bool DBin::isSupportFixed(vector<bool> const &fixmask) const
{
   return fixmask[1]; //SIZE is fixed;
} 
