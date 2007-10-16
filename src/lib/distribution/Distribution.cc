#include <config.h>
#include <distribution/Distribution.h>
#include <util/dim.h>

using std::vector;
using std::string;

Distribution::Distribution(string const &name, unsigned int npar, 
			   bool canbound, bool discrete)
    : _name(name), _npar(npar), _canbound(canbound), _discrete(discrete)
{
}

Distribution::~Distribution()
{
}

string const &Distribution::name() const
{
  return _name;
}

bool Distribution::isDiscreteValued() const
{
  return _discrete;
}

unsigned int Distribution::npar() const
{
  return _npar;
}

bool Distribution::canBound() const
{
  return _canbound;
}

bool Distribution::checkParameterValue(vector<double const *> const &args,
           vector<vector<unsigned int> > const &dims) const
{
  return true;
}

vector<unsigned int>
Distribution::dim(vector<vector<unsigned int> > const &dims) const
{
    return vector<unsigned int>(1,1);
}

unsigned int Distribution::df(vector<vector<unsigned int> > const &pdims) const
{
    return product(dim(pdims));
}


bool Distribution::isLocationParameter(unsigned int i) const
{
    return false;
}

bool Distribution::isScaleParameter(unsigned int i) const
{
    return false;
}
