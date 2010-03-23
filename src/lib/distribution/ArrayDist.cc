#include <config.h>
#include <distribution/ArrayDist.h>
#include <util/dim.h>

using std::string;
using std::vector;

ArrayDist::ArrayDist(string const &name, unsigned int npar)
  : Distribution(name, npar)
{
}

unsigned int ArrayDist::df(vector<vector<unsigned int> > const &pdims) const
{
    return product(dim(pdims));
}

