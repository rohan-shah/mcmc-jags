#include <config.h>
#include <sarray/util.h>
#include "Mean.h"

using std::vector;

Mean::Mean ()
    : Function ("mean", 1)
{
}

void Mean::evaluate (double *x, vector<double const*> const &args,
                     vector<vector<unsigned int> > const &dims) const
{
    unsigned int len = product(dims[0]);
    double svalue = 0;
    for (unsigned int i = 0; i < len; i++) {
       svalue += args[0][i];
    }
    svalue /= len;
    *x = svalue;
}

bool Mean::checkParameterDim (vector<vector<unsigned int> > const &dims) const
{
    return true;
}

bool Mean::isLinear(vector<bool> const &mask, vector<bool> const &fix) const
{
    return true;
}

bool Mean::isScale(unsigned int index, vector<bool> const &fix) const
{
    return true;
}

