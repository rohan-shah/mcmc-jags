#include <config.h>
#include "AuxMixBinomial.h"
#include "LGMix.h"
#include <JRmath.h>
#include <rng/RNG.h>
#include <cmath>

using std::exp;

namespace glm {
    
    AuxMixBinomial::AuxMixBinomial(double const &eta, double const &nb, 
				   double const &y)
	: _eta(eta), _nb(nb), _y(y)
    {
	_mix = new LGMix(nb);
    }

    AuxMixBinomial::~AuxMixBinomial()
    {
	delete _mix;
    }

    void AuxMixBinomial::update(RNG *rng)
    {
	if (_nb == 0) 
	    return;

	// sample the aggregated utility 
	double lambda = exp(_eta);
	
	double u = rgamma(_nb, 1.0, rng);
	double v = 0.0;
	if (static_cast<int>(_y) < static_cast<int>(_nb)) {
	    v = rgamma(_nb - _y, 1.0, rng);
	} 
	_y_star = -log(u / (1.0 + lambda) + v / lambda);
	
	// ...then the mixture representation 
	_mix->update(_y_star - _eta, _nb, rng);
    }

    double AuxMixBinomial::value() const
    {
	if (_nb == 0) {
	    return 0;
	}
	else {
	    return _y_star - _mix->mean();
	}
    }
    
    double AuxMixBinomial::precision() const
    {
	if (_nb == 0) {
	    return 0;
	}
	else {
	    return _mix->precision();
	}
    }
}
