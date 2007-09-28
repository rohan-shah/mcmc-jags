#include <config.h>
#include <sarray/util.h>
#include "Dim.h"

using std::vector;

namespace base {

    Dim::Dim () : Function("dim", 1)
    {
    }

    vector<unsigned int> Dim::dim(vector <vector<unsigned int> > const &dims) 
	const
    {
	return vector<unsigned int>(1, dims[0].size());

    }

    void Dim::evaluate(double *x, vector <double const *> const &args,
			  vector<unsigned int> const &lengths,
			  vector<vector<unsigned int> > const &dims) const
    {
	for (unsigned int i = 0; i < dims[0].size(); ++i) {
	    x[i] = dims[0][i];
	}
    }

    bool Dim::isDiscreteValued(vector<bool> const &mask) const
    {
	return true;
    }

    bool Dim::checkParameterDim(vector<vector<unsigned int> > const &dims) const
    {
	return true;
    }

}
