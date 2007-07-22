#include <config.h>
#include "Equal.h"

#include <cmath>
#include <cfloat>

using std::vector;

Equal::Equal () : Infix ("==")
{
}

double Equal::eval (vector<double const *> const &args) const
{
  return *args[0] == *args[1];
}

bool Equal::isDiscreteValued(std::vector<bool> const &mask) const
{
  return true;
}
