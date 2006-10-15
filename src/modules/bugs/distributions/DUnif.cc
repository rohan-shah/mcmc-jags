#include <config.h>
#include <sarray/SArray.h>
#include "DUnif.h"

#include <JRmath.h>

using std::vector;

static inline double LOWER (vector<SArray const *> const &par)
{
    return *par[0]->value();
}


static inline double UPPER (vector<SArray const *> const &par)
{
    return *par[1]->value();
}

DUnif::DUnif()
  : DistReal("dunif", 2, DIST_SPECIAL, false)
{}


DUnif::~DUnif()
{}

bool 
DUnif::checkParameterValue (vector<SArray const *> const &par) const
{
    return (LOWER(par) < UPPER(par));
}

double
DUnif::d(double x, vector<SArray const *> const &par, bool give_log) const
{
    return dunif(x, LOWER(par), UPPER(par), give_log);
}

double
DUnif::p(double q, vector<SArray const *> const &par, bool lower, bool log_p)
  const
{
    return punif(q, LOWER(par), UPPER(par), lower, log_p);
}

double
DUnif::q(double p, vector<SArray const *> const &par, bool lower, bool log_p)
  const
{
    return qunif(p, LOWER(par), UPPER(par), lower, log_p);
}

double DUnif::r(vector<SArray const *> const &par, RNG *rng) const
{
    return runif(LOWER(par), UPPER(par), rng);
}

double DUnif::l(vector<SArray const*> const &par) const
{
    return LOWER(par);
}

double DUnif::u(vector<SArray const*> const &par) const
{
    return UPPER(par);
}

bool DUnif::isSupportFixed(vector<bool> const &fixmask) const
{
    return fixmask[0] && fixmask[1]; //Lower and upper bounds fixed
}
