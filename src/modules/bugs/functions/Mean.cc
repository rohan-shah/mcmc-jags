#include <config.h>
#include <sarray/util.h>
#include "Mean.h"

using std::vector;

Mean::Mean ()
    : Function ("mean", 1)
{
}

void Mean::evaluate (double *x, vector<double const*> const &args,
		     vector<unsigned int> const &lengths,
                     vector<vector<unsigned int> > const &dims) const
{
    double svalue = 0;
    for (unsigned int i = 0; i < lengths[0]; i++) {
	svalue += args[0][i];
    }
    svalue /= lengths[0];
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

