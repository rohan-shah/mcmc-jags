#include <config.h>
#include "ArcSin.h"

#include <cmath>

using std::vector;
using std::asin;

namespace bugs {

    ArcSin::ArcSin ()
	: ScalarFunction ("arcsin", 1)
    {
    }
    
    double ArcSin::evaluate(vector<double const *> const &args) const
    {
	return asin(*args[0]);
    }

    bool ArcSin::checkParameterValue(vector<double const *> const &args) const
    {
	return *args[0] >= -1 && *args[0] <= 1;
    }
}
