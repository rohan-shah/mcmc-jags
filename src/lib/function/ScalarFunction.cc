#include <config.h>
#include <function/ScalarFunction.h>
#include <util/dim.h>

#include <algorithm>

using std::vector;
using std::string;
using std::find_if;

ScalarFunction::ScalarFunction (string const &name, unsigned int npar)
  : Function (name, npar)
{
}

bool 
ScalarFunction::checkParameterValue(vector<double const *> const &args) const
{
    return true;
}

bool ScalarFunction::isPower(vector<bool> const &mask,
			     vector<bool> const &isfixed) const
{
    unsigned int nmask = 0;
    for (unsigned int i = 0; i < mask.size(); ++i) {
	nmask += mask[i];
    }
    
    if (nmask > 1)
	return false;
    else
	return isScale(mask, vector<bool>());
    
}
