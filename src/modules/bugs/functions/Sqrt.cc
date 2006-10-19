#include <config.h>
#include "Sqrt.h"

#include <cmath>

using std::vector;
using std::sqrt;

Sqrt::Sqrt ():ScalarFunc ("sqrt", 1)
{
}

double Sqrt::eval(vector<double const *> const &args) const
{
  return sqrt(*args[0]);
}

bool Sqrt::checkParameterValue(vector<double const *> const &args) const
{
  return *args[0] >= 0;
}
