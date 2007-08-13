#include <config.h>
#include <sarray/util.h>
#include "Neg.h"

#include <stdexcept>
#include <cmath>

using std::vector;
using std::logic_error;
using std::string;

namespace basefunctions {

Neg::Neg () : ScalarFunc ("NEG", 1)
{
}

double Neg::evaluateScalar(vector<double const*> const &args) const
{
    return -args[0][0];
}

bool Neg::isDiscreteValued(vector<bool> const &mask) const
{
  return allTrue(mask);
}

bool Neg::isLinear(vector<bool> const &mask, vector<bool> const &fix) const
{
    return true;
}

bool Neg::isScale(unsigned int index, vector<bool> const &fix) const
{
    return true;
}

string Neg::deparse(vector<string> const &par) const
{
    return string("-") + par[0];
}

}
