#include <config.h>
#include <distribution/VectorDist.h>

using std::string;
using std::vector;

VectorDist::VectorDist(string const &name, unsigned int npar)
  : Distribution(name, npar)
{
}

unsigned int VectorDist::df(vector<unsigned int> const &par) const
{
    return length(par);
}
