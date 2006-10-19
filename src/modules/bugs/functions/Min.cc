#include <config.h>
#include <sarray/util.h>
#include "Min.h"

#include <algorithm>

using std::min_element;
using std::min;
using std::vector;

Min::Min ()
  : Function ("min", 0)
{
}

void Min::evaluate(double *value,  vector<double const *> const &args,
                     vector<vector<unsigned int> > const &dims) const
{
  double ans;
  for (unsigned int i = 0; i < args.size(); ++i) {
    unsigned int len = product(dims[i]);
    double mini = *min_element(args[i], args[i] + len);
    if (i == 0) {
      ans = mini;
    }
    else {
      ans = min(ans, mini);
    }
  }
  *value = ans;
}

bool Min::checkParameterDim (vector<vector<unsigned int> > const &dims) const
{
  return true;
}

bool Min::isDiscreteValued(vector<bool> const &mask) const
{
    return allTrue(mask);
}

