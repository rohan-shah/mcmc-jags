#include <config.h>

#include "PFunction.h"

#include <distribution/RScalarDist.h>

using std::vector;
using std::string;

namespace bugs {

    PFunction::PFunction(RScalarDist const *dist)
	: DPQFunction(string("p") + dist->name().substr(1), dist)
    {}
    
    double PFunction::evaluate(vector<double const *> const &args) const
    {
	double x = *args[0];
	vector<double const *> param(args.size() - 1);
	for (unsigned int i = 1; i < args.size(); ++i) {
	    param[i-1] = args[i];
	}
	
	return dist()->p(x, param, true, false);
    }

    bool 
    PFunction::checkParameterValue(vector<double const *> const &args) const
    {
	return checkArgs(args);
    }
}
