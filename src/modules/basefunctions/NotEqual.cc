#include <config.h>
#include "NotEqual.h"

#include <cmath>
#include <cfloat>

using std::vector;

NotEqual::NotEqual () : Infix ("!=")
{
}

double NotEqual::eval (vector<double const *> const &args) const
{
  return *args[0] != *args[1];
}

bool NotEqual::isDiscreteValued(std::vector<bool> const &mask) const
{
  return true;
}
