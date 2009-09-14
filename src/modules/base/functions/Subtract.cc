#include <config.h>
#include <util/logical.h>
#include "Subtract.h"

#include <stdexcept>

using std::vector;
using std::logic_error;

namespace base {

Subtract::Subtract () : Infix ("-")
{
}

double
Subtract::evaluateScalar(vector<double const *> const &args) const
{
    return *args[0] - *args[1];
}

bool Subtract::isDiscreteValued(vector<bool> const &mask) const
{
  return allTrue(mask);
}

bool 
Subtract::isScale(vector<bool> const &mask, vector<bool> const &fix) const
{
    return allTrue(mask);
}

}
