#include <config.h>
#include "matrix.h"
#include <sarray/util.h>
#include "Inverse.h"

using std::vector;

Inverse::Inverse ():Function ("inverse", 1)
{
}

void Inverse::evaluate (double *value, vector<double const *> const &args,
                        vector<vector<unsigned int> > const &dims) const
{
    inverse (value, args[0], dims[0][0], false);
}

bool Inverse::checkParameterDim (vector<vector<unsigned int> > const &dims) const
{
    return isSquareMatrix(dims[0]);
}

vector<unsigned int> Inverse::dim (vector<vector<unsigned int> > const &dims) const
{
    return dims[0];
}
