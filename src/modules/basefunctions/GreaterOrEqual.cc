#include <config.h>
#include "GreaterOrEqual.h"

#include <cmath>
#include <cfloat>

using std::vector;

GreaterOrEqual::GreaterOrEqual () : Infix (">=")
{
}

double GreaterOrEqual::eval (vector<double const *> const &args) const
{
  return *args[0] >= *args[1];
}

bool GreaterOrEqual::isDiscreteValued(std::vector<bool> const &mask) const
{
  return true;
}
