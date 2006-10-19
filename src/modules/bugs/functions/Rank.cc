#include <config.h>
#include <sarray/util.h>
#include "Rank.h"

#include <algorithm>

using std::vector;
using std::stable_sort;

bool lt_doubleptr (double const *arg1, double const *arg2) {
  return *arg1 < *arg2;
}

Rank::Rank ()
  : Function ("rank", 1)
{
}

void Rank::evaluate(double *value, vector<double const *> const &args,
                    vector<vector<unsigned int> > const &dims) const
{
  long len = product(dims[0]);

  //Create a vector of pointers to the elements of arg and sort it
  double const **argptrs = new double const *[len];
  for (long i = 0; i < len; ++i) {
    argptrs[i] = args[0] + i;
  }
  stable_sort(argptrs, argptrs + len, lt_doubleptr);

  //Ranks can be inferred from the sorted vector of pointers
  for (long i = 0; i < len; ++i) {
    value[argptrs[i] - args[0]] = i + 1;
  }
  delete [] argptrs;
}

vector<unsigned int>  Rank::dim (vector <vector<unsigned int> > const &dims) const
{
  return dims[0];
}

bool Rank::checkParameterDim (vector<vector<unsigned int> > const &dims) const
{
    return isVector(dims[0]) || isScalar(dims[0]);
}

bool Rank::isDiscreteValued(vector<bool> const &mask) const
{
  return true;
}
