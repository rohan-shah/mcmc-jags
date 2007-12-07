#include <config.h>
#include <stdexcept>
#include <cmath>
#include <cfloat>

using std::runtime_error;

static const double eps = 16 * DBL_EPSILON;

int asInteger(double fval)
{
    if (fval >= INT_MAX || fval <= INT_MIN) {
	throw runtime_error("double value out of range for conversion to int");
    }
    int ival;
    if (fval > 0) {
	ival = static_cast<int>(fval + eps);
    }
    else {
	ival = static_cast<int>(fval - eps);
    }
    return ival;
}

int checkInteger(double fval, bool &flag)
{
    int ival = asInteger(fval);
    flag = fabs(fval - ival) < eps;
    return ival;
}

