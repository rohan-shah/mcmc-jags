#include <config.h>
#include <sarray/util.h>
#include "Max.h"

#include <algorithm>

using std::vector;
using std::max_element;
using std::max;

Max::Max ()
  : Function("max", 0)
{
}

void Max::evaluate(double *value,  vector<double const *> const &args,
                     vector<vector<unsigned int> > const &dims) const
{
  double ans;
  for (unsigned int i = 0; i < args.size(); ++i) {
    unsigned int len = product(dims[i]);
    double maxi = *max_element(args[i], args[i] + len);
    if (i == 0) {
      ans = maxi;
    }
    else {
      ans = max(ans, maxi);
    }
  }
  *value = ans;
}

bool Max::checkParameterDim (vector<vector<unsigned int> > const &dims) const
{
  return true;
}

bool Max::isDiscreteValued(vector<bool> const &mask) const
{
    return allTrue(mask);
}

