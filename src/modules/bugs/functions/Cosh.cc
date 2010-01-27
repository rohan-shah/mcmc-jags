#include <config.h>
#include "Cosh.h"

#include <cmath>

using std::vector;
using std::cosh;

namespace bugs {

    Cosh::Cosh ()
	: ScalarFunction ("cosh", 1)
    {
    }

    double Cosh::evaluate(vector<double const *> const &args) const
    {
	return cosh(*args[0]);
    }

}
