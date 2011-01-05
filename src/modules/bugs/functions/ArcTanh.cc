#include <config.h>
#include "ArcTanh.h"

#include <cmath>

using std::vector;
using std::log;
using std::string;

namespace bugs {

    ArcTanh::ArcTanh ()
	: ScalarFunction ("arctanh", 1)
    {
    }

    string ArcTanh::alias() const
    {
	return "atanh";
    }

    double ArcTanh::evaluate(vector<double const *> const &args) const
    {
	double x = *args[0];
	return (log(1 + x) - log(1 - x))/2;
    }

        
    bool ArcTanh::checkParameterValue(vector<double const *> const &args) const
    {
	return *args[0] > -1 && *args[0] <  1;
    }

}
