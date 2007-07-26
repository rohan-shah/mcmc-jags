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
		     vector<unsigned int> const &lengths, 
                     vector<vector<unsigned int> > const &dims) const
{
  for (unsigned int i = 0; i < lengths[0]; ++i) {
    value[i] = args[0][i];
  }
  sort(value, value + lengths[0]);
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
