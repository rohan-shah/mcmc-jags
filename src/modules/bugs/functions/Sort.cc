#include <config.h>
#include "Sort.h"

#include <algorithm>
#include <stdexcept>

#include <sarray/util.h>

using std::vector;
using std::sort;
using std::logic_error;

Sort::Sort ()
  : Function ("sort", 1)
{
}

void Sort::evaluate (double *value, vector <double const *> const &args,
                     vector<vector<unsigned int> > const &dims) const
{
  long len = product(dims[0]);
  for (unsigned int i = 0; i < len; ++i) {
    value[i] = args[0][i];
  }
  sort(value, value + len);
}

vector<unsigned int>  Sort::dim (vector <vector<unsigned int> > const &dims) const
{
    return dims[0];
}

bool Sort::checkParameterDim (vector<vector<unsigned int> > const &dims) const
{
    return isVector(dims[0]) || isScalar(dims[0]);
}

bool Sort::isDiscreteValued(vector<bool> const &mask) const
{
  return count(mask.begin(), mask.end(), false) == 0;
}
