#include <config.h>
#include <util/logical.h>
#include "InProd.h"

#include "lapack.h"

using std::vector;

namespace bugs {

    InProd::InProd () : VectorFunction ("inprod", 2)
    {
    }

    void InProd::evaluate(double *x, vector<double const *> const &args,
			  vector<unsigned int> const &lengths) const
    {
        int one = 1, N = lengths[0];
        *x = F77_DDOT(&N, args[0], &one, args[1], &one);
    }

    bool 
    InProd::checkParameterLength (vector<unsigned int> const &lengths) const
    {
	return (lengths[0] == lengths[1]);
    }

    bool InProd::isDiscreteValued(vector<bool> const &mask) const
    {
	return allTrue(mask);
    }

    bool 
    InProd::isScale(vector<bool> const &mask, vector<bool> const &fix) const
    {
	//Test for quadratic term
	if (mask[0] && mask[1])
	    return false;

	if (fix.empty()) {
	    return true;
	}
        else {
	    return (mask[0] || fix[0]) && (mask[1] || fix[1]); 
        }
    }
}
