#include <config.h>
#include <function/InverseLinkFunc.h>

using std::string;

InverseLinkFunc::InverseLinkFunc (string const &name, string const &link)
  : Function (name, 1), _link(link)
{
}

string const & InverseLinkFunc::linkName () const
{
  return _link;
}

