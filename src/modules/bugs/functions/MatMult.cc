#include <config.h>

#include "MatMult.h"
#include <sarray/util.h>

using std::vector;

MatMult::MatMult()
  : Function("%*%", 2)
{
}

void 
MatMult::evaluate (double *value, vector<double const *> const &args,
		   vector<unsigned int> const &lengths,
		   vector<vector<unsigned int> > const &dims) const
{
    unsigned int d1, d2, d3;

    if (dims[0].size() == 1) {
	d1 = 1;
	d2 = dims[0][0];
    }
    else {
	d1 = dims[0][0];
	d2 = dims[0][1];
    }
    if (dims[1].size() == 1) {
	d3 = 1;
    }
    else {
	d3 = dims[1][1];
    }
    
    for(unsigned int i = 0; i < d1; ++i) {
	for (unsigned int j = 0; j < d3; ++j) {
	    value[i + d1*j] = 0;
	    for (unsigned int k = 0; k < d2; ++k) {
		value[i + d1*j] += args[0][i + d1*k] * args[1][k + d2*j];
	    }
	}
    }
}

vector<unsigned int> 
MatMult::dim (vector <vector<unsigned int> > const &dims) const
{
    vector<unsigned int> ans(2,1);

    if (dims[0].size() == 2) {
	ans[0] = dims[0][0];
    }
    if (dims[1].size() == 2) {
	ans[1] = dims[1][1];
    }

    return drop(ans);
}

bool 
MatMult::checkParameterDim (vector<vector<unsigned int> > const &dims) const
{
    if ((dims[0].size() > 2 || dims[1].size() > 2)) {
	return false;
    }

    if (dims[0].size() == 1) {
	return dims[0][0] == dims[1][0];
    }
    else {
	return dims[0][1] == dims[1][0];
    }
}


bool 
MatMult::isLinear(vector<bool> const &mask, vector<bool> const &fix) const
{
    //Test for quadratic terms
    if (mask[0] && mask[1]) {
	return false;
    }
    
    if (!fix.empty()) {
	for (unsigned int i = 0; i < 2; ++i) {
	    if (!mask[i] && !fix[i]) {
		return false;
	    }
	}
    }

    return true;
}
