#include <config.h>
#include "Tanh.h"

#include <cmath>

using std::vector;
using std::tanh;

namespace bugs {

    Tanh::Tanh ()
	: ScalarFunction ("tanh", 1)
    {
    }

    double Tanh::evaluate(vector<double const *> const &args) const
    {
	return tanh(*args[0]);
    }

}
