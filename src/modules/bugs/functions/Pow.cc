#include <config.h>
#include "Pow.h"

#include <cmath>
#include <cfloat>

using std::vector;
using std::fabs;
using std::pow;

Pow::Pow ()
  : ScalarFunc ("pow", 2)
{
}

double Pow::evaluateScalar(vector<double const *> const &args) const
{
    return pow (*args[0], *args[1]);
}

bool Pow::checkScalarValue(vector<double const *> const &args) const
{
    if (*args[0] >= 0) {
	return true;
    }
    else {
	double arg2 = *args[1];
	int iarg2 = static_cast<int>(arg2 + DBL_EPSILON);
	return fabs(arg2 - iarg2) < DBL_EPSILON;
    }
}
