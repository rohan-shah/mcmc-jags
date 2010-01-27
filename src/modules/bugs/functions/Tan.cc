#include <config.h>
#include "Tan.h"

#include <cmath>

using std::vector;
using std::tan;

namespace bugs {


    Tan::Tan ()
	: ScalarFunction ("tan", 1)
    {
    }

    double Tan::evaluate(vector<double const *> const &args) const
    {
	return tan(*args[0]);
    }

}
