#include <config.h>
#include <function/ArrayFunction.h>

using std::vector;
using std::string;

ArrayFunction::ArrayFunction (string const &name, unsigned int npar)
    : Function(name, npar)
{
}

bool 
ArrayFunction::checkParameterValue(vector<double const *> const &args,
				   vector<vector<unsigned int> > const &dims) 
    const
{
    return true;
}
