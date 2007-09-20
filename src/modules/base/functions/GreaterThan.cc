#include <config.h>
#include "GreaterThan.h"

#include <cmath>
#include <cfloat>

using std::vector;

namespace base {

GreaterThan::GreaterThan () : Infix (">")
{
}

double GreaterThan::evaluateScalar(vector<double const *> const &args) const
{
  return *args[0] > *args[1];
}

bool GreaterThan::isDiscreteValued(std::vector<bool> const &mask) const
{
  return true;
}

}
