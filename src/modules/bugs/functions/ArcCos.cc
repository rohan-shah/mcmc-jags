#include <config.h>
#include "ArcCos.h"

#include <cmath>

using std::vector;
using std::acos;
using std::string;

namespace bugs {

    ArcCos::ArcCos ()
	: ScalarFunction ("arccos", 1)
    {
    }

    string ArcCos::alias() const
    {
	return "acos";
    }
    
    double ArcCos::evaluate(vector<double const *> const &args) const
    {
	return acos(*args[0]);
    }

    bool ArcCos::checkParameterValue(vector<double const *> const &args) const
    {
	return *args[0] >= -1 && *args[0] <= 1;
    }
}
