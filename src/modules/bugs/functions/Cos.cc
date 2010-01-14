#include <config.h>
#include "Cos.h"

#include <cmath>

using std::vector;
using std::cos;

namespace bugs {

    Cos::Cos ()
	: ScalarFunction ("cos", 1)
    {
    }

    double Cos::evaluate(vector<double const *> const &args) const
    {
	return cos(*args[0]);
    }

}
