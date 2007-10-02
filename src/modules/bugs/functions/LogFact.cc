#include <config.h>
#include "LogFact.h"

#include <JRmath.h>

using std::vector;

namespace bugs {

    LogFact::LogFact ()
	: ScalarFunc ("logfact", 1)
    {
    }

    double LogFact::evaluateScalar(vector<double const *> const &args) const
    {
	return lgammafn(*args[0] + 1);
    }

    bool LogFact::checkScalarValue(vector<double const *> const &args) const
    {
	return *args[0] > -1;
    }

}
