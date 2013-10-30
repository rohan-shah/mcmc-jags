#include <config.h>
#include "CLogLog.h"

#include <cmath>

using std::vector;
using std::log;

namespace jags {
namespace bugs {

    CLogLog::CLogLog ():ScalarFunction ("cloglog", 1)
    {
    }

    double CLogLog::evaluate(vector<double const *> const &args) const
    {
	return log(-log(1 - *args[0]));
    }

    bool CLogLog::checkParameterValue(vector<double const *> const &args) const
    {
	double p = *args[0];
	return (p >= 0 && p <= 1);
    }

}}
