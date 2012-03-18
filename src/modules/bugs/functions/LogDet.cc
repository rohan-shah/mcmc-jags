#include <config.h>

#include "LogDet.h"
#include "matrix.h"

#include <util/dim.h>
#include <cmath>

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
	return isSquareMatrix(dims[0]) || isScalar(dims[0]);
    }

    vector<unsigned int>
    LogDet::dim(vector<vector<unsigned int> > const &dims) const
    {
	return vector<unsigned int>(1,1);
    }


    bool 
    LogDet::checkParameterValue(vector<double const *> const &args,
				vector<vector<unsigned int> > const &dims) const
    {
	return check_symmetry(args[0], dims[0][0]);
    }

}

