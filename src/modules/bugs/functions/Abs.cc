#include <config.h>
#include "Abs.h"

#include <cmath>

using std::vector;

Abs::Abs ():ScalarFunc ("abs", 1)
{
}

double Abs::eval(vector<double const *> const &args) const
{
    return fabs(*args[0]);
}


bool Abs::isDiscreteValued(vector<bool> const &mask) const
{
    return mask[0];
}
