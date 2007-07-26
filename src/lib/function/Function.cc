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
                                   vector<unsigned int> const &lengths,
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
Function::dim(vector <vector<unsigned int> > const &dims) const
{
    return vector<unsigned int>(1,1);
}

string Function::deparse(vector<string> const &par) const
{
    string name = this->name();
    name.append("(");
    for (unsigned int i = 0; i < par.size(); ++i) {
	if (i > 0) {
	    name.append(",");
	}
	name.append(par[i]);
    }
    name.append(")");

    return name;
}
