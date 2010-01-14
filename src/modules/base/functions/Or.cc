#include <config.h>
#include "Or.h"

using std::vector;

namespace base {

Or::Or () : Infix ("||")
{
}

double Or::evaluate(vector <double const *> const &args) const
{
  return *args[0] || *args[1];
}

bool Or::isDiscreteValued(std::vector<bool> const &mask) const
{
  return true;
}

}
