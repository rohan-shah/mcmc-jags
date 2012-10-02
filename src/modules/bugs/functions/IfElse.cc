#include <config.h>
#include "IfElse.h"

using std::vector;

namespace bugs {

    IfElse::IfElse () : ScalarFunction("ifelse", 3)
    {
    }
    
    double IfElse::evaluate(vector<double const *> const &args) const
    {
	return *args[0] ? *args[1] : *args[2];
    }
    
    bool IfElse::isDiscreteValued(vector<bool> const &mask) const
    {
	return mask[1] && mask[2];
    }

}
