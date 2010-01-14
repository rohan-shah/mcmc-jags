#include <config.h>

#include "Mexp.h"
#include "matexp.h"

#include <util/dim.h>

using std::vector;

namespace msm {

Mexp::Mexp()
    : ArrayFunction("mexp",1)
{
}

void Mexp::evaluate (double *value, vector<double const *> const &args,
                     vector<vector<unsigned int> > const &dims) const
{
    MatrixExpPade(value, args[0], dims[0][0], 1);
}

vector<unsigned int> Mexp::dim (vector <vector<unsigned int> > const &dims) const
{
    return dims[0];
}

bool Mexp::checkParameterDim (vector <vector<unsigned int> > const &dims) const
{
    return isSquareMatrix(dims[0]);
}

}
