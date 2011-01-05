#include <config.h>
#include "DNegBin.h"

#include <cmath>
#include <vector>
#include <algorithm>

#include <JRmath.h>

using std::vector;
using std::max;
using std::string;

#define PROB(par) (*par[0])
#define SIZE(par) (*par[1])

DNegBin::DNegBin()
    : RScalarDist("dnegbin", 2, DIST_POSITIVE, true)
{}

string DNegBin::alias() const
{
    return "dnbinom";
}

bool 
DNegBin::checkParameterValue (vector<double const *> const &par) const
{
    double p = PROB(par);
    return (SIZE(par) > 0 && p > 0 && p < 1);
}

double
DNegBin::d(double x, vector<double const *> const &par, bool give_log) 
    const
{
  return dnbinom(x, SIZE(par), PROB(par), give_log);
}

double
DNegBin::p(double q, vector<double const *> const &par, bool lower, 
	   bool give_log) const
{
    return pnbinom(q, SIZE(par), PROB(par), lower, give_log);
}

double 
DNegBin::q(double p, vector<double const *> const &par, bool lower, 
	   bool log_p) const
{
    return qnbinom(p, SIZE(par), PROB(par), lower, log_p);
}

double DNegBin::r(vector<double const *> const &par, RNG *rng) const
{
    return rnbinom(SIZE(par), PROB(par), rng);
}

