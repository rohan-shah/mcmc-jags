#include <config.h>
#include <util/logical.h>
#include "DSumFunc.h"

using std::vector;

namespace bugs {

    DSumFunc::DSumFunc()
	: ScalarFunction("dsum", 0)
    {
    }
    
    double DSumFunc::evaluate(vector<double const *> const &args) const
    {
	double ans = 0;
	for (unsigned int i = 0; i < args.size(); ++i) {
	    ans += *args[i];
	}
	return ans;
    }
    
    bool DSumFunc::isDiscreteValued(vector<bool> const &mask) const
    {
	return allTrue(mask);
    }

}
