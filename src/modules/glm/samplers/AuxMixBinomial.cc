#include <config.h>

#include "AuxMixBinomial.h"
#include "LGMix.h"
#include "Classify.h"

#include <JRmath.h>
#include <rng/RNG.h>
#include <graph/StochasticNode.h>
#include <module/ModuleError.h>

#include <cmath>

using std::exp;

namespace jags {
namespace glm {

    static const double one = 1;

    static double const &getDenominator(StochasticNode const *snode,
					unsigned int chain)
    {
	switch(getFamily(snode)) {
	case GLM_BERNOULLI:
	    return one;
	case GLM_BINOMIAL:
	    return snode->parents()[1]->value(chain)[0];
	default:
	    throwLogicError("Invalid outcome in AuxMixBinomial");
	}

	return one; //Wall
    }
    

    AuxMixBinomial::AuxMixBinomial(StochasticNode const *snode,
				   unsigned int chain)
	: Outcome(snode, chain), _nb(getDenominator(snode, chain)), 
	  _y(snode->value(chain)[0]), _y_star(0), _mix(0)
    {
	_mix = new LGMix(_nb);
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
	double lambda = exp(_lp);
	
	double u = rgamma(_nb, 1.0, rng);
	double v = 0.0;
	if (static_cast<int>(_y) < static_cast<int>(_nb)) {
	    v = rgamma(_nb - _y, 1.0, rng);
	} 
	_y_star = -log(u / (1.0 + lambda) + v / lambda);
	
	// ...then the mixture representation 
	_mix->update(_y_star - _lp, _nb, rng);
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

    bool AuxMixBinomial::canRepresent(StochasticNode const *snode) 
    {
	return (getFamily(snode) == GLM_BINOMIAL ||
		getFamily(snode) == GLM_BERNOULLI) &&
	    getLink(snode) == LNK_LOGIT;
    }

}}

