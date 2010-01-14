#include <config.h>
#include "InterpLin.h"

#include <util/nainf.h>
#include <util/dim.h>

using std::vector;

namespace bugs {

    InterpLin::InterpLin() : VectorFunction("interp.lin", 3)
    {}
    
    void InterpLin::evaluate(double *value, vector<double const *> const &args,
			     vector<unsigned int> const &lengths) const
    {
	double const *x = args[1];
	double const *y = args[2];
	double xnew = args[0][0];
	unsigned int N = lengths[1];
    
	if (xnew < x[0])
	    value[0] = y[0];
	else if (xnew >= x[N-1])
	    value[0] = y[N-1];
	else {
	    unsigned int i = 0;
	    for (; i < N-1; ++i) {
		if (xnew >= x[i] && xnew < x[i+1]) {
		    break;
		}
	    }
	    if (i == N-1) {
		/* This shouldn't happen, but we must guard against an
		   attempt to access an illegal element of x or y */
		value[0] = JAGS_NAN;
	    }
	    else {
		value[0] = y[i] + 
		    (xnew - x[i]) * (y[i+1] - y[i]) / (x[i+1] - x[i]);
	    }
	}
    }

    bool InterpLin::checkParameterLength(vector<unsigned int> const &lengths)
	const
    {
	return (lengths[0] == 1) && (lengths[1] == lengths[2]);
    }

    bool 
    InterpLin::checkParameterValue(vector <double const *> const &args,
				   vector <unsigned int> const &lengths) const
    {
        unsigned int N = lengths[1];
	for (unsigned int i = 1; i < N; ++i) {
	    if (args[1][i] < args[1][i-1])
		return false;
	}
	if (args[0][0] < args[1][0] || args[0][0] > args[1][N-1])
	    return false;

	return true;
    }

}
