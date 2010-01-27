#include <config.h>
#include "Sinh.h"

#include <cmath>

using std::vector;
using std::sinh;

namespace bugs {

    Sinh::Sinh ()
	: ScalarFunction ("sinh", 1)
    {
    }

    double Sinh::evaluate(vector<double const *> const &args) const
    {
	return sinh(*args[0]);
    }

}
