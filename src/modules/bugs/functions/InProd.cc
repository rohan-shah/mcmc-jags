#include <config.h>
#include <util/logical.h>
#include "InProd.h"

using std::vector;

namespace bugs {

    InProd::InProd () : Function ("inprod", 2)
    {
    }

    void InProd::evaluate(double *x, vector<double const *> const &args,
			  vector<unsigned int> const &lengths,
			  vector<vector<unsigned int> > const &dims) const
    {
	double svalue = 0;
	for (long i = 0; i < lengths[0]; i++)
	{
	    svalue += args[0][i] * args[1][i];
	}
	*x = svalue;
    }

    bool 
    InProd::checkParameterDim (vector<vector<unsigned int> > const &dims) const
    {
	return (dims[0] == dims[1]);
    }

    bool InProd::isDiscreteValued(std::vector<bool> const &mask) const
    {
	return allTrue(mask);
    }

    bool 
    InProd::isScale(vector<bool> const &mask, vector<bool> const &fix) const
    {
	//Test for quadratic term
	if (mask[0] && mask[1])
	    return false;

	if (!fix.empty()) {
	    if ( (!mask[0] && !fix[0]) || (!mask[1] && !fix[1]) )
		return false;
	}

	return true;
    }

    bool
    InProd::isScaleMix(vector<bool> const &mask, vector<bool> const &fix) const
    {
	return false;
    }
}
