#include <config.h>
#include "Log.h"

#include <cmath>

using std::vector;
using std::log;

namespace bugs {

    Log::Log ()
	: ScalarFunction ("log", 1)
    {
    }

    double Log::evaluate(vector<double const *> const &args) const
    {
	return log(*args[0]);
    }

    bool Log::checkScalarValue(vector<double const *> const &args) const
    {
	return *args[0] >= 0;
    }

}
