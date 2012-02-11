#include <config.h>
#include <util/nainf.h>
#include "KLBern.h"

#include <cmath>

using std::vector;
using std::log;

#define PROB0 (*par0[0])
#define PROB1 (*par1[0])

namespace dic {

    double KLBern::divergence(vector<double const *> const &par0,
			      vector<double const *> const &par1) const
    {
	if (PROB0 == 0) {
	    return - log(1 - PROB1);
	}
	else if (PROB0 == 1) {
	    return - log(PROB1);
	}
	else {
	    return (PROB0 * (log(PROB0) - log(PROB1)) +
		    (1 - PROB0) * (log(1 - PROB0) - log(1 - PROB1)));
	}
    }

}
