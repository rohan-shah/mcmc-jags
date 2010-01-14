#include <config.h>
#include "LogGam.h"

#include <JRmath.h>

using std::vector;

namespace bugs {

    LogGam::LogGam ()
	: ScalarFunction ("loggam", 1)
    {
    }

    double LogGam::evaluate(vector<double const *> const &args) const
    {
	return lgammafn (*args[0]);
    }

    bool LogGam::checkScalarValue(vector<double const *> const &args) const
    {
	return *args[0] > 0;
    }

}
