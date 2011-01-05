#include <config.h>
#include "ArcTan.h"

#include <cmath>

using std::vector;
using std::atan;
using std::string;

namespace bugs {

    ArcTan::ArcTan ()
	: ScalarFunction ("arctan", 1)
    {
    }

    string ArcTan::alias() const
    {
	return "atan";
    }
    
    double ArcTan::evaluate(vector<double const *> const &args) const
    {
	return atan(*args[0]);
    }

}
