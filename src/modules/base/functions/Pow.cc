#include <config.h>
#include "Pow.h"

#include <cmath>
#include <cfloat>

using std::vector;
using std::fabs;
using std::pow;

namespace base {

Pow::Pow () : Infix ("^")
{
}

double Pow::evaluate(vector<double const *> const &args) const
{
    return pow (*args[0], *args[1]);
}

bool Pow::checkParameterValue(vector<double const *> const &args) const
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

    bool Pow::isPower(vector<bool> const &mask, vector<bool> const &fix) const
    {
	if (mask[1])
	    return false;
        else
            return fix.empty() || fix[1];
    }
}
