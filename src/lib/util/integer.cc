#include <config.h>
#include <stdexcept>
#include <cmath>

using std::runtime_error;
using std::cmath;

int asInteger(double fval)
{
    static const eps = sqrt(DBL_EPSILON);

    if (fval >= INT_MAX || fval <= INT_MIN) {
	throw runtime_error("double value out of range for conversion to int");
    }
    int ival;
    if (fval > 0) {
	ival = (int) (fval + eps);
    }
    else {
	ival = (int) (fval - eps);
    }
    if (fabs(fval - ival) > eps) {
	throw runtime_error("Invalid integer conversion");
    }
    return ival;
}
