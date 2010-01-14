#include <config.h>
#include "DSumFunc.h"

using std::vector;

namespace bugs {

    DSumFunc::DSumFunc()
	: ScalarFunction("dsum", 2)
    {
    }
    
    double DSumFunc::evaluate(vector<double const *> const &args) const
    {
	return *args[0] + *args[1];
    }

}
