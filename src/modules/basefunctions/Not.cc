#include <config.h>
#include "Not.h"

using std::vector;
using std::string;

namespace basefunctions {

Not::Not () : ScalarFunc ("!", 1)
{
}

double Not::eval (vector<double const*> const &args) const
{
    return *args[0] != 0;
}

bool Not::isDiscreteValued(vector<bool> const &mask) const
{
    return true;
}

string Not::deparse(vector<string> const &par) const
{
    return string("!") + par[0];
}

}
