#include <config.h>
#include "SD.h"

#include <cmath>

using std::vector;
using std::sqrt;

namespace bugs {

    SD::SD ()
	: VectorFunction ("sd", 1)
    {
    }

    void SD::evaluate (double *x, vector<double const *>const &args,
		       vector<unsigned int> const &lengths) const
    {
	double svalue = 0;
	if (lengths[0] > 1) {
	    unsigned int i;
	    double sum = 0;
	    for (unsigned i = 0; i < lengths[0]; i++) {
		sum += args[0][i];
	    }
	    double mean = sum / lengths[0];

	    double var = 0;
	    for (i = 0; i < lengths[0]; i++) {
		var += (args[0][i] - mean) * (args[0][i] - mean);
	    }
	    svalue = sqrt (var / lengths[0]);
	}
	*x = svalue;
    }

    bool SD::checkParameterLength (vector<unsigned int> const &lengths) const
    {
	return lengths[0] >= 2;
    }

}
