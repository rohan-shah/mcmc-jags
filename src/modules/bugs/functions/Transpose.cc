#include <config.h>
#include <util/dim.h>

#include "Transpose.h"

using std::vector;

namespace jags {
namespace bugs {

    Transpose::Transpose()
	: ArrayFunction("t",1)
    {
    }

    void
    Transpose::evaluate (double *value, vector<double const *> const &args,
			 vector<vector<unsigned int> > const &dims) const
    {
	unsigned int nrow = dims[0][0];
	unsigned int ncol = dims[0].size() == 2 ? dims[0][1] : 1;
	unsigned int length = nrow * ncol;
	for (unsigned int i = 0; i < length; ++i) {
	    value[i] = args[0][(i / ncol) + (i % ncol) * nrow];
	}
    }

    vector<unsigned int> 
    Transpose::dim (vector <vector<unsigned int> > const &dims,
		    vector <double const *> const &values) const
    {
	vector<unsigned int> ans(2);
	ans[0] = dims[0].size() == 2 ? dims[0][1] : 1;
	ans[1] = dims[0][0];
	return ans;
    }

    bool 
    Transpose::checkParameterDim (vector <vector<unsigned int> > const &dims) 
	const
    {
	return isScalar(dims[0]) || isVector(dims[0]) || isMatrix(dims[0]);
    }

    bool Transpose::isAdditive(vector<bool> const &mask,
			       vector<bool> const &isfixed) const
    {
	return true;
    }
    
    bool Transpose::isScale(vector<bool> const &mask,
		            vector<bool> const &isfixed) const
    {
	return true;
    }

    bool Transpose::isDiscreteValued(std::vector<bool> const &mask) const
    {
	return mask[0];
    }
}}
