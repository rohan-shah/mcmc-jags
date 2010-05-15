#include <config.h>

#include "Transpose.h"

using std::vector;

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
    Transpose::dim (vector <vector<unsigned int> > const &dims) const
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
	return dims[0].size() == 1 || dims[0].size() == 2;
    }

    bool Transpose::isScale(vector<bool> const &mask,
		            vector<bool> const &isfixed) const
    {
	return true;
    }
    
}
