#include <config.h>
#include "Probit.h"

#include <JRmath.h>

using std::vector;

Probit::Probit ()
  : ScalarFunc ("probit", 1)
{
}

double Probit::eval(vector<double const *> const &args) const
{
    return qnorm5 (*args[0], 0, 1, 0, 0);
}

bool Probit::checkParameterValue (vector <double const *> const &args) const
{
    double p = *args[0];
    return (p > 0 && p < 1);
}
