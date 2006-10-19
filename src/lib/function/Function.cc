#include <config.h>
#include <function/Function.h>

using std::vector;
using std::string;

Function::Function (string const &name, unsigned int npar)
  : _name (name), _npar (npar)
{
}

Function::~Function ()
{
  //virtual destructor
}

string const & Function::name () const
{
  return _name;
}

bool Function::checkParameterValue(vector<double const *> const &args,
			           vector<vector<unsigned int> > const &dims)
const
{
  return true;
}

bool Function::checkParameterLength (unsigned int npar) const
{
  return _npar == 0 ||  npar == _npar;
}

bool Function::isDiscreteValued(vector<bool> const &mask) const
{
  return false;
}

bool Function::isLinear(vector<bool> const &mask,
			vector<bool> const &isfixed) const
{
  return false;
}

bool Function::isScale(unsigned int index,
		       vector<bool> const &isfixed) const
{
    return false;
}

vector<unsigned int> 
Function::dim(std::vector <std::vector<unsigned int> > const &dims) const
{
    return vector<unsigned int>(1,1);
}
