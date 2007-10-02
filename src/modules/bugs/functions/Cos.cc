#include <config.h>
#include "Cos.h"

#include <cmath>

using std::vector;
using std::cos;

namespace bugs {

    Cos::Cos ()
	: ScalarFunc ("cos", 1)
    {
    }

    double Cos::evaluateScalar(vector<double const *> const &args) const
    {
	return cos(*args[0]);
    }

}
