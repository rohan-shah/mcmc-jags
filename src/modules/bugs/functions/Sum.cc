#include <config.h>
#include <sarray/util.h>
#include "Sum.h"

#include <stdexcept>

using std::vector;
using std::logic_error;

Sum::Sum () : Function("sum", 1)
{
}

void Sum::evaluate(double *x, vector <double const *> const &args,
	      vector<vector<unsigned int> > const &dims) const
{
    unsigned int len = product(dims[0]);
    double value = 0;
    for (long i = 0; i < len; i++) {
	value += args[0][i];
    }
    *x = value;
}

bool Sum::checkParameterDim (vector<vector<unsigned int> > const &args) const
{
    return true;
}

bool Sum::isDiscreteValued(vector<bool> const &mask) const
{
    return count(mask.begin(), mask.end(), false) == 0;
}

bool Sum::isLinear(vector<bool> const &mask, vector<bool> const &fix) const
{
  return true;
}

bool Sum::isScale(unsigned int index, vector<bool> const &fix) const
{
  return true;
}
