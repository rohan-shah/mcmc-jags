#include <config.h>
#include <util/logical.h>
#include <util/dim.h>
#include "DSumFunc.h"

using std::vector;

namespace bugs {

    DSumFunc::DSumFunc()
	: ArrayFunction("dsum", 0)
    {
    }
    
    void DSumFunc::evaluate(double *x,
			    vector<double const *> const &args,
			    vector<vector<unsigned int> > const &dims) const
    {
	unsigned int length = product(dims[0]);
	for (unsigned int i = 0; i < length; ++i) {
	    x[i] = 0;
	    for (unsigned int j = 0; j < args.size(); ++j) {
		x[i] += args[j][i];
	    }
	}
    }
    
    bool DSumFunc::isDiscreteValued(vector<bool> const &mask) const
    {
	return allTrue(mask);
    }

    bool 
    DSumFunc::checkParameterDim(vector<vector<unsigned int> > const &dims) const
    {
	for (unsigned int i = 1; i < dims.size(); ++i) {
	    if (dims[i] != dims[0])
		return false;
	}
	return true;
    }

    vector<unsigned int>  
    DSumFunc::dim(vector<vector<unsigned int> > const &dims) const
    {
	return dims[0];
    }

}
