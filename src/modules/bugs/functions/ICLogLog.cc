#include <config.h>
#include "ICLogLog.h"

#include <cmath>

using std::vector;
using std::log;
using std::exp;

namespace bugs {

    ICLogLog::ICLogLog(): LinkFunction("icloglog", "cloglog")
    {
    }

    double ICLogLog::inverseLink(double eta) const
    {
	return 1 - exp(-exp(eta));
    }

    double ICLogLog::link(double mu) const
    {
	return log (-log (1 - mu));
    }

    double ICLogLog::grad(double eta) const
    {
	return exp(eta) * exp(-exp(eta));
    }

}
