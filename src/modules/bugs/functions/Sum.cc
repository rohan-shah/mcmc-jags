#include <config.h>
#include <sarray/util.h>
#include "Sum.h"

using std::vector;

Sum::Sum () : Function("sum", 1)
{
}

void Sum::evaluate(double *x, vector <double const *> const &args,
	      vector<vector<unsigned int> > const &dims) const
{
    double value = 0;
    unsigned int len = product(dims[0]);
    for (unsigned long i = 0; i < len; ++i) {
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
    return allTrue(mask);
}

bool Sum::isLinear(vector<bool> const &mask, vector<bool> const &fix) const
{
  return true;
}

bool Sum::isScale(unsigned int index, vector<bool> const &fix) const
{
  return true;
}
