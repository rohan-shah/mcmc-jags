#include <config.h>
#include "Sqrt.h"

#include <cmath>

using std::vector;
using std::sqrt;

namespace bugs {

    Sqrt::Sqrt ():ScalarFunction ("sqrt", 1)
    {
    }

    double Sqrt::evaluate(vector<double const *> const &args) const
    {
	return sqrt(*args[0]);
    }

    bool Sqrt::checkScalarValue(vector<double const *> const &args) const
    {
	return *args[0] >= 0;
    }

    bool Sqrt::isPower(vector<bool> const &mask, vector<bool> const &fix) const
    {
        return true;
    }


}
