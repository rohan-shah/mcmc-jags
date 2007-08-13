#include <config.h>
#include "And.h"

using std::vector;

namespace basefunctions {

And::And () : Infix ("&&")
{
}

double And::evaluateScalar(vector <double const *> const &args) const
{
  return *args[0] && *args[1];
}

bool And::isDiscreteValued(std::vector<bool> const &mask) const
{
  return true;
}

}
