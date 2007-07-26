#include <config.h>

#include "LogDet.h"
#include "matrix.h"

#include <sarray/util.h>

using std::vector;

LogDet::LogDet ()
  : Function ("logdet", 1)
{
}

void LogDet::evaluate (double *x, vector<double const *> const &args,
		       vector<unsigned int> const &lengths,
                       vector<vector<unsigned int> > const &dims) const
{
  *x = logdet(args[0], dims[0][0]);
}

bool LogDet::checkParameterDim (vector<vector<unsigned int> > const &dims) const
{
  return isSquareMatrix(dims[0]);
}

/* FIXME: we need a checkParameterValue function */

