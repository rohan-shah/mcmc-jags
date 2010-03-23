
#include <config.h>

#include <distribution/RScalarDist.h>

#include "DFunction.h"

using std::vector;
using std::string;

namespace bugs {

    DFunction::DFunction(RScalarDist const *dist)
	: DPQFunction(dist->name(), dist)
    {}
    
    double DFunction::evaluate(vector<double const *> const &args) const
    {
	double x = *args[0];
	vector<double const *> param(args.size() - 1);
	for (unsigned int i = 1; i < args.size(); ++i) {
	    param[i-1] = args[i];
	}
	
	return dist()->d(x, param, false);
    }

    bool 
    DFunction::checkParameterValue(vector<double const *> const &args) const
    {
	double x = *args[0];
	if (dist()->isDiscreteValued()) {
	    //Work around printed warning by Rmath library
	    int ix = static_cast<int>(x);
	    if (x != ix)
		return 0;
	}

	vector<double const *> param(args.size() - 1);
	for (unsigned int i = 1; i < args.size(); ++i) {
	    param[i-1] = args[i];
	}
	
	if (x < dist()->l(param) || x > dist()->u(param)) {
	    return false;
	}
	
	return checkArgs(args);
    }
}
