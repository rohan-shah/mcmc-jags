#include <config.h>
#include "DUnif.h"

#include <JRmath.h>

using std::vector;

#define LOWER(par) (*par[0])
#define UPPER(par) (*par[1])

DUnif::DUnif()
    : DistScalarRmath("dunif", 2, DIST_SPECIAL, false, false)
{}

bool 
DUnif::
checkParameterValue (vector<double const *> const &par,
		     vector<vector<unsigned int> > const &dims) const
{
    return (LOWER(par) < UPPER(par));
}

double
DUnif::d(double x, vector<double const *> const &par, bool give_log) const
{
    return dunif(x, LOWER(par), UPPER(par), give_log);
}

double
DUnif::p(double q, vector<double const *> const &par, bool lower, bool log_p)
  const
{
    return punif(q, LOWER(par), UPPER(par), lower, log_p);
}

double
DUnif::q(double p, vector<double const *> const &par, bool lower, bool log_p)
  const
{
    return qunif(p, LOWER(par), UPPER(par), lower, log_p);
}

double DUnif::r(vector<double const *> const &par, RNG *rng) const
{
    return runif(LOWER(par), UPPER(par), rng);
}

double DUnif::l(vector<double const*> const &par) const
{
    return LOWER(par);
}

double DUnif::u(vector<double const*> const &par) const
{
    return UPPER(par);
}

bool DUnif::isSupportFixed(vector<bool> const &fixmask) const
{
    return fixmask[0] && fixmask[1]; //Lower and upper bounds fixed
}
