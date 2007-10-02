#include <config.h>
#include "Sin.h"

#include <cmath>

using std::vector;
using std::sin;

namespace bugs {


    Sin::Sin ()
	: ScalarFunc ("sin", 1)
    {
    }

    double Sin::evaluateScalar(vector<double const *> const &args) const
    {
	return sin(*args[0]);
    }

}
