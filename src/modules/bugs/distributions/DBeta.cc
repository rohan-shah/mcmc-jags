#include <config.h>
#include "DBeta.h"

#include <JRmath.h>

using std::vector;

DBeta::DBeta()
    : DistScalarRmath("dbeta", 2, DIST_PROPORTION, true, false)
{}

bool DBeta::checkParameterValue(vector<double const *> const &par) const
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
