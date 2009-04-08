#include <config.h>
#include "DIntervalFunc.h"
#include <util/dim.h>

using std::vector;

#define T(args) (*args[0])
#define CUTPOINTS(args) (args[1])
#define NCUT(dims) (dims[1][0])

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

    DIntervalFunc::DIntervalFunc () : Function ("dinterval", 2)
    {
    }

    void 
    DIntervalFunc::evaluate(double *x, vector<double const *> const &args,
			    vector<unsigned int> const &lengths,
			    vector<vector<unsigned int> > const &dims) const
    {
	*x = value(args, NCUT(dims));
    }
    
    bool 
    DIntervalFunc::checkParameterDim (vector<vector<unsigned int> > const &dims)
	const
    {
	return isScalar(dims[0]) && dims[1].size() == 1;
    }
    
    bool DIntervalFunc::isDiscreteValued(std::vector<bool> const &mask) const
    {
	return true;
    }
    
}
