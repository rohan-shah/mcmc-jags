#include <config.h>
#include "ICLogLog.h"

#include <cmath>

using std::vector;
using std::log;
using std::exp;

namespace bugs {

    ICLogLog::ICLogLog(): InverseLinkFunc("icloglog", "cloglog")
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

    double ICLogLog::gradLink(double mu) const
    {
	return -1/((1-mu)*log(1-mu));
    }

}
