#include <config.h>
#include "ILogit.h"

#include <cmath>
#include <cfloat>

using std::vector;
using std::exp;
using std::log;

ILogit::ILogit ()
    : InverseLinkFunc ("ilogit", "logit")
{
}

double ILogit::eval (vector<double const *> const &args) const
{
    return 1/(1 + exp(-args[0][0]));
}

double ILogit::link(double mu) const
{
  return log(mu) - log(1- mu);
}

double ILogit::gradLink(double mu) const
{
  return 1/(mu*(1-mu));
}
