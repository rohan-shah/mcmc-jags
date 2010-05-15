#include <config.h>
#include "DIntervalFunc.h"
#include <util/dim.h>

using std::vector;

#define T(args) (*args[0])
#define CUTPOINTS(args) (args[1])

static unsigned int value(vector<double const *> const &args, unsigned int ncut)
{
    double t = T(args);
    for (unsigned int i = 0; i < ncut; ++i) {
	if (t <= CUTPOINTS(args)[i])
	    return i;
    }
    return ncut;
}

namespace bugs {

    DIntervalFunc::DIntervalFunc () : VectorFunction ("dinterval", 2)
    {
    }

    void 
    DIntervalFunc::evaluate(double *x, vector<double const *> const &args,
			    vector<unsigned int> const &lengths) const
    {
	*x = value(args, lengths[1]);
    }
    
    bool DIntervalFunc::checkParameterLength (vector<unsigned int> const &args)
	const
    {
	return args[0] == 1;
    }
    
    bool DIntervalFunc::isDiscreteValued(vector<bool> const &mask) const
    {
	return true;
    }
    
}
