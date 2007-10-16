#include <config.h>
#include <util/logical.h>
#include "Add.h"

using std::vector;

namespace base {

Add::Add () : Infix ("+", 0)
{
}

double Add::evaluateScalar(vector <double const *> const &args) const
{
    double out = *args[0];
    unsigned int n = args.size();
    for (unsigned int i = 1; i < n; ++i) {
	out += *args[i];
    }
    return out;
}

bool Add::isDiscreteValued(vector<bool> const &mask) const
{
    return allTrue(mask);
}

bool Add::isLinear(vector<bool> const &mask, vector<bool> const &fix) const
{
    return true;
}

}
