#include <config.h>
#include <util/logical.h>
#include "Add.h"

using std::vector;

namespace base {

    Add::Add () : Infix ("+", 0)
    {
    }
    
    double Add::evaluate(vector <double const *> const &args) const
    {
	double out = *args[0];
	for (unsigned int i = 1; i < args.size(); ++i) {
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

    bool Add::isScale(vector<bool> const &mask, vector<bool> const &fix) const
    {
	return allTrue(mask);
    }
}
