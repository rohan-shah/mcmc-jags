#include <config.h>
#include "CLogLog.h"

#include <cmath>

using std::vector;
using std::log;

namespace bugs {

    CLogLog::CLogLog ():ScalarFunction ("cloglog", 1)
    {
    }

    double CLogLog::evaluate(vector<double const *> const &args) const
    {
	return log(-log(1 - *args[0]));
    }

    bool CLogLog::checkScalarValue(vector<double const *> const &args) const
    {
	double arg = *args[0];
	return (arg > 0 && arg < 1);
    }

}
