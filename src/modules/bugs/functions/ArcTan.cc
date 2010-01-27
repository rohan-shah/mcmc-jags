#include <config.h>
#include "ArcTan.h"

#include <cmath>

using std::vector;
using std::atan;

namespace bugs {

    ArcTan::ArcTan ()
	: ScalarFunction ("arctan", 1)
    {
    }
    
    double ArcTan::evaluate(vector<double const *> const &args) const
    {
	return atan(*args[0]);
    }

}
