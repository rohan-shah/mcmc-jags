#include <config.h>
#include <sarray/util.h>
#include "SD.h"

#include <cmath>

using std::vector;
using std::sqrt;

SD::SD ()
  : Function ("sd", 1)
{
}

void SD::evaluate (double *x, vector<double const *>const &args,
		   vector<vector<unsigned int> > const &dims) const
{
    unsigned int len = product(dims[0]);
    double svalue = 0;
    if (len > 1) {
	unsigned int i;
	double sum = 0;
	for (unsigned i = 0; i < len; i++) {
	    sum += args[0][i];
	}
	double mean = sum / len;

	double var = 0;
	for (i = 0; i < len; i++) {
	    var += (args[0][i] - mean) * (args[0][i] - mean);
	}
	svalue = sqrt (var / len);
    }
    *x = svalue;
}

bool SD::checkParameterDim (vector<vector<unsigned int> > const &dims) const
{
  return dims[0][0] >= 2;
}
