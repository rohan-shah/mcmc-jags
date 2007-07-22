#include <config.h>
#include "Or.h"

using std::vector;

namespace basefunctions {

Or::Or () : Infix ("||")
{
}

double Or::eval(vector <double const *> const &args) const
{
  return *args[0] || *args[1];
}

bool Or::isDiscreteValued(std::vector<bool> const &mask) const
{
  return true;
}

}
