#include <config.h>
#include <function/InverseLinkFunc.h>

using std::string;
using std::vector;

InverseLinkFunc::InverseLinkFunc (string const &name, string const &link)
  : ScalarFunc (name, 1), _link(link)
{
}

string const &
InverseLinkFunc::linkName () const
{
  return _link;
}

double 
InverseLinkFunc::evaluateScalar(vector <double const *> const &args) const
{
    return inverseLink(args[0][0]);
}
