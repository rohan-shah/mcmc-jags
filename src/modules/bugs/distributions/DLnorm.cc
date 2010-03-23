#include <config.h>
#include "DLnorm.h"

#include <cmath>

#include <JRmath.h>

using std::vector;

#define MU(par) (*par[0])
#define TAU(par) (*par[1])
#define SDLOG(par) (1/sqrt(*par[1]))

DLnorm::DLnorm()
    : RScalarDist("dlnorm", 2, DIST_POSITIVE)
{}

bool DLnorm::checkParameterValue (vector<double const *> const &par) const
{
    return TAU(par) > 0;
}

double 
DLnorm::d(double x, vector<double const *> const &par, bool give_log) const
{
    return dlnorm(x, MU(par), SDLOG(par), give_log);
}

double 
DLnorm::p(double q, vector<double const *> const &par, bool lower, 
	  bool give_log) const
{
    return plnorm(q, MU(par), SDLOG(par), lower, give_log);
}

double 
DLnorm::q(double p, vector<double const *> const &par, bool lower, bool log_p)
    const
{
    return qlnorm(p, MU(par), SDLOG(par), lower, log_p);
}

double 
DLnorm::r(vector<double const *> const &par, RNG *rng) const

{
    return rlnorm(MU(par), SDLOG(par), rng);
}
