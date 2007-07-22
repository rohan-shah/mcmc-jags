#include <config.h>
#include "LessThan.h"

#include <cmath>
#include <cfloat>

using std::vector;

namespace basefunctions {

LessThan::LessThan () : Infix ("<")
{
}

double LessThan::eval (vector<double const *> const &args) const
{
  return *args[0] < *args[1];
}

bool LessThan::isDiscreteValued(std::vector<bool> const &mask) const
{
  return true;
}

}
