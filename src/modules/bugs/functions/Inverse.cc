#include <config.h>
#include "matrix.h"
#include <util/dim.h>
#include "Inverse.h"

using std::vector;

namespace bugs {

    Inverse::Inverse ():Function ("inverse", 1)
    {
    }

    void Inverse::evaluate (double *value, vector<double const *> const &args,
			    vector<unsigned int> const &lengths,
			    vector<vector<unsigned int> > const &dims) const
    {
	inverse (value, args[0], dims[0][0], false);
    }

    bool 
    Inverse::checkParameterDim (vector<vector<unsigned int> > const &dims) const
    {
	return isSquareMatrix(dims[0]);
    }

    vector<unsigned int> 
    Inverse::dim (vector<vector<unsigned int> > const &dims) const
    {
	return dims[0];
    }

}
