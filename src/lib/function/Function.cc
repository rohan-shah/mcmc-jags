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

string Function::alias() const
{
    return "";
}

bool Function::checkNPar (unsigned int npar) const
{
  return _npar == 0 ||  npar == _npar;
}

bool Function::isDiscreteValued(vector<bool> const &mask) const
{
  return false;
}

bool Function::isScale(vector<bool> const &mask,
		       vector<bool> const &isfixed) const
{
    return false;
}

bool Function::isLinear(vector<bool> const &mask,
			vector<bool> const &isfixed) const
{
    return isScale(mask, isfixed);
}

bool 
Function::isPower(vector<bool> const &mask, vector<bool> const &isfixed) const
{
    return false;
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
