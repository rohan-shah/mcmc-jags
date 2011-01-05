#include <config.h>
#include <distribution/Distribution.h>
#include <util/dim.h>

using std::vector;
using std::string;

Distribution::Distribution(string const &name, unsigned int npar)
    : _name(name), _npar(npar)
{
}

Distribution::~Distribution()
{
}

string const &Distribution::name() const
{
  return _name;
}

string Distribution::alias() const
{
    return "";
}

bool Distribution::checkNPar (unsigned int npar) const
{
  return _npar == 0 ||  npar == _npar;
}

bool Distribution::isLocationParameter(unsigned int i) const
{
    return false;
}

bool Distribution::isScaleParameter(unsigned int i) const
{
    return false;
}

bool Distribution::isDiscreteValued(vector<bool> const &mask) const
{
    return false;
}

bool Distribution::canBound() const
{
    return false;
}

bool Distribution::checkParameterDiscrete(vector<bool> const &mask) const
{
    return true;
}
