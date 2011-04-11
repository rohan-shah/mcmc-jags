#include <config.h>
#include <util/logical.h>
#include "Subtract.h"

using std::vector;

namespace base {

    Subtract::Subtract () : Infix ("-")
    {
    }
    
    double
    Subtract::evaluate(vector<double const *> const &args) const
    {
	return *args[0] - *args[1];
    }
    
    bool Subtract::isDiscreteValued(vector<bool> const &mask) const
    {
	return allTrue(mask);
    }
    
    bool 
    Subtract::isScale(vector<bool> const &mask, vector<bool> const &fix) const
    {
	return allTrue(mask);
    }
    
    bool 
    Subtract::isLinear(vector<bool> const &mask, vector<bool> const &fix) const
    {
	return true;
    }
}
