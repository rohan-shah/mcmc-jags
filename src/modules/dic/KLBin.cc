#include <config.h>
#include <util/nainf.h>
#include "KLBin.h"

#include <cmath>

using std::vector;
using std::log;

#define PROB0 (*par0[0])
#define PROB1 (*par1[0])
#define N0 (*par0[1])
#define N1 (*par1[1])

namespace dic {

    double KLBin::divergence(vector<double const *> const &par0,
			     vector<double const *> const &par1) const
    {
	//Note: ignoring a term depending only on N, which is
	//expensive to calculate and cancels out when we take the
	//opposite divergence

	if (N0 != N1) {
	    return JAGS_POSINF;
	}
	else if (PROB0 == 0) {
	    return - N0 * log(1 - PROB1);
	}
	else if (PROB0 == 1) {
	    return - N0 * log(PROB1);
	}
	else {
	    return (N0 * PROB0 * (log(PROB0) - log(PROB1)) +
		    N0 * (1 - PROB0) * (log(1 - PROB0) - log(1 - PROB1)));
	}
    }

}
