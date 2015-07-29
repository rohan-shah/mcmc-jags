#include <config.h>
#include "DIntervalFunc.h"
#include <util/dim.h>

using std::vector;

#define T(args) (*args[0])
#define CUTPOINTS(args) (args[1])

namespace jags {
namespace bugs {

    DIntervalFunc::DIntervalFunc () : ScalarVectorFunction ("dinterval", 2)
    {
    }

    double DIntervalFunc::scalarEval(vector<double const *> const &args,
				     vector<unsigned int> const &lengths) const
    {
	unsigned int ncut = lengths[1];
	double t = T(args);
	for (unsigned int i = 0; i < ncut; ++i) {
	    if (t <= CUTPOINTS(args)[i])
		return i;
	}
	return ncut;
    }
    
    bool DIntervalFunc::checkParameterLength (vector<unsigned int> const &args)
	const
    {
	return args[0] == 1 && args[1] >= 1;
    }
    
    bool DIntervalFunc::isDiscreteValued(vector<bool> const &mask) const
    {
	return true;
    }
    
}}
