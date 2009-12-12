#include <config.h>
#include "AuxMixNormal.h"

namespace glm {
    
    AuxMixNormal::AuxMixNormal(double const &tau, double const &y)
	: _tau(tau), _y(y)
    {
    }

    void AuxMixNormal::update(RNG *rng)
    {
	//Nothing to do
    }

    double AuxMixNormal::value() const
    {
	return _y;
    }
    
    double AuxMixNormal::precision() const
    {
	return _tau;
    }

}
