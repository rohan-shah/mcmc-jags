#include <config.h>
#include <function/VectorFunction.h>

using std::vector;
using std::string;

VectorFunction::VectorFunction (string const &name, unsigned int npar)
    : Function(name, npar)
{
}

bool 
VectorFunction::checkParameterValue(vector<double const *> const &args,
				    vector<unsigned int> const &lengths) const
{
    return true;
}

unsigned int 
VectorFunction::length(vector <unsigned int> const &arglengths) const
{
    return 1;
}

bool 
VectorFunction::checkParameterLength(vector<unsigned int> const &args) const
{
    return true;
}
