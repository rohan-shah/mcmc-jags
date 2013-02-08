#include <config.h>
#include "DRoundFunc.h"
#include <JRmath.h>

using std::vector;

#define T(args) (*args[0])
#define NDIGITS(args) (*args[1])

namespace jags {
namespace bugs {

    DRoundFunc::DRoundFunc () : ScalarFunction ("dround", 2)
    {
    }

    double DRoundFunc::evaluate(vector<double const *> const &args) const
    {
	return(fround(T(args), NDIGITS(args)));
    }

}}
