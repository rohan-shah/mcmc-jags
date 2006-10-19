#include <config.h>
#include "DWeib.h"

#include <vector>
#include <cmath>

#include <JRmath.h>

using std::vector;

#define SHAPE(par) (*par[0])
#define SCALE(par) (pow(*par[1], -1/(*par[0])))

DWeib::DWeib()
    : DistScalarRmath("dweib", 2, DIST_POSITIVE, true, false)
{}


bool 
DWeib::checkParameterValue (vector<double const *> const &par) const
{
    return (*par[0] > 0 && *par[1] > 0);
}

double 
DWeib::d(double x, vector<double const *> const &par, bool give_log)
    const
{
    return dweibull(x, SHAPE(par), SCALE(par), give_log);
}

double
DWeib::p(double q, vector<double const *> const &par, bool lower, bool give_log)
  const
{
    return pweibull(q, SHAPE(par), SCALE(par), lower, give_log);
}

double 
DWeib::q(double p, vector<double const *> const &par, bool lower, bool log_p)
    const
{
    return qweibull(p, SHAPE(par), SCALE(par), lower, log_p);
}

double DWeib::r(vector<double const *> const &par, RNG *rng) const
{
    return rweibull(SHAPE(par), SCALE(par), rng);
}
