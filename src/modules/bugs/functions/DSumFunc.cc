#include <config.h>
#include <util/logical.h>
#include <util/dim.h>
#include <util/logical.h>
#include "DSumFunc.h"

using std::vector;

namespace jags {
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
	if (dims.empty()) return false;
	if (isFlat(dims[0])) return false;
	for (unsigned int j = 1; j < dims.size(); ++j) {
	    if (dims[j] != dims[0]) return false;
	}
	return true;
    }

    vector<unsigned int>  
    DSumFunc::dim(vector<vector<unsigned int> > const &dims,
		  vector<double const *> const &values) const
    {
	return dims[0];
    }

    bool DSumFunc::isLinear(vector<bool> const &mask, 
			    vector<bool> const &fixed) const
    {
	return true;
    }

    bool DSumFunc::isScale(vector<bool> const &mask, 
			   vector<bool> const &fixed) const
    {
	return allTrue(mask);
    }

}}
