#include <config.h>
#include "DRoundFunc.h"
#include <JRmath.h>

using std::vector;

#define T(args) (*args[0])
#define NDIGITS(args) (*args[1])

namespace bugs {

    DRoundFunc::DRoundFunc () : ScalarFunction ("dround", 2)
    {
    }

    double DRoundFunc::evaluate(vector<double const *> const &args) const
    {
	return(fprec(T(args), NDIGITS(args)));
    }

}
