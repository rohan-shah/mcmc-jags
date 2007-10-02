#include <config.h>

#include "Transpose.h"
#include <sarray/util.h>

using std::vector;

namespace bugs {

    Transpose::Transpose()
	: Function("t",1)
    {
    }

    void
    Transpose::evaluate (double *value, vector<double const *> const &args,
			 vector<unsigned int> const &lengths,
			 vector<vector<unsigned int> > const &dims) const
    {
	unsigned int nrow = dims[0][0];
	unsigned int ncol = dims[0].size() == 2 ? dims[0][1] : 1;
	for (unsigned int i = 0; i < lengths[0]; ++i) {
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

}
