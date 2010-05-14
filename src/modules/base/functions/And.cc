#include <config.h>
#include "And.h"

using std::vector;

namespace base {

And::And () : Infix ("&&")
{
}

double And::evaluate(vector <double const *> const &args) const
{
  return *args[0] && *args[1];
}

bool And::isDiscreteValued(vector<bool> const &mask) const
{
  return true;
}

}
