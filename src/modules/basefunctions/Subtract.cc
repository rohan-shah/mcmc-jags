#include <config.h>
#include <sarray/util.h>
#include "Subtract.h"

#include <stdexcept>

using std::vector;
using std::logic_error;

Subtract::Subtract () : Infix ("-")
{
}

double
Subtract::eval(vector<double const *> const &args) const
{
    return *args[0] - *args[1];
}

bool Subtract::isDiscreteValued(vector<bool> const &mask) const
{
  return allTrue(mask);
}

bool 
Subtract::isLinear(vector<bool> const &mask, vector<bool> const &fix) const
{
    return true;
}
