#include <config.h>

#include "LogDet.h"
#include "matrix.h"

#include <util/dim.h>

using std::vector;

namespace bugs {

    LogDet::LogDet ()
	: ArrayFunction ("logdet", 1)
    {
    }

    void LogDet::evaluate (double *x, vector<double const *> const &args,
			   vector<vector<unsigned int> > const &dims) const
    {
	*x = logdet(args[0], dims[0][0]);
    }

    bool 
    LogDet::checkParameterDim (vector<vector<unsigned int> > const &dims) const
    {
	return isSquareMatrix(dims[0]);
    }

    vector<unsigned int>
    LogDet::dim(vector<vector<unsigned int> > const &dims) const
    {
	return vector<unsigned int>(1,1);
    }
}

/* FIXME: we need a checkParameterValue function */

