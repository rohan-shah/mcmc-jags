#include <config.h>
#include "Logit.h"

#include <cmath>

using std::vector;
using std::log;

Logit::Logit ():ScalarFunc ("logit", 1)
{
}

double Logit::eval (vector <double const *> const &args) const
{
  double arg = *args[0];
  return log (arg / (1 - arg));
}

bool Logit::checkParameterValue (vector <double const *> const &args) const
{
  double arg = *args[0];
  return (arg >= 0 && arg <= 1);
}
