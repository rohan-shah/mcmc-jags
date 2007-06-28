#include <config.h>
#include <function/ScalarFunc.h>
#include <sarray/util.h>

#include <algorithm>

using std::vector;
using std::string;
using std::find_if;

ScalarFunc::ScalarFunc (string const &name, unsigned int npar)
  : Function (name, npar)
{
}

void
ScalarFunc::evaluate (double *value, vector<double const *> const &args,
                      vector<vector<unsigned int> > const &dims) const
{
  *value = eval(args);
}

vector<unsigned int> 
ScalarFunc::dim(vector<vector<unsigned int> > const &args) const
{
  return vector<unsigned int>(1,1);
}

bool
ScalarFunc::checkParameterDim(vector<vector<unsigned int> > const &dims) const
{
  return count_if(dims.begin(), dims.end(), isScalar) == dims.size();
}

bool 
ScalarFunc::checkParameterValue(vector<double const *> const &args,
				vector<vector<unsigned int> > const &dims)
    const
{
    return checkParameterValue(args);
}

bool ScalarFunc::checkParameterValue(vector<double const *> const &args) const
{
    return true;
}
