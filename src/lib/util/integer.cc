#include <config.h>
#include <stdexcept>
#include <cmath>
#include <cfloat>
#include <climits>
#include <string>

using std::runtime_error;
using std::fabs;
using std::string;

static const double eps = 16 * DBL_EPSILON;

static int coerceInteger(double fval)
{
    if (fval > 0) {
	return static_cast<int>(fval + eps);
    }
    else {
	return static_cast<int>(fval - eps);
    }
}

int asInteger(double fval)
{
    if (fval >= INT_MAX || fval <= INT_MIN) {
	throw runtime_error(string("double value out of range for conversion to int"));
    }
    return coerceInteger(fval);
}

bool checkInteger(double fval)
{
    if (fval >= INT_MAX || fval <= INT_MIN) {
	return false;
    }
    return fabs(fval - coerceInteger(fval)) < eps;
}

