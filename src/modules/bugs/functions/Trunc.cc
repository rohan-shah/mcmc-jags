#include <config.h>
#include "Trunc.h"

#include <cmath>

using std::vector;
using std::floor;

Trunc::Trunc ():ScalarFunc ("trunc", 1)
{
}

double Trunc::eval(vector<double const *> const &args) const
{
  /* The C99 trunc function does not exist in the current C++ standard. */
  return args[0][0] >= 0 ? floor(args[0][0]) : -floor(-args[0][0]);
}
