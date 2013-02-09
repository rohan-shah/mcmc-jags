#include <config.h>
#include "AuxMixPoisson.h"
#include "LGMix.h"
#include "Classify.h"

#include <rng/RNG.h>
#include <graph/StochasticNode.h>
#include <JRmath.h>

#include <cmath>

using std::exp;

namespace jags {
namespace glm {

    AuxMixPoisson::AuxMixPoisson(StochasticNode const *snode, unsigned int chain)
	: Outcome(snode, chain), _y(snode->value(chain)[0]), _mix1(0), _mix2(0), _tau1(0), _tau2(0)
    {
	_mix1 = new LGMix(1);
	_mix2 = new LGMix(_y);
    }

    AuxMixPoisson::~AuxMixPoisson()
    {
	delete _mix1;
	delete _mix2;
    }

    void AuxMixPoisson::update(RNG * rng)
    {
	double lambda = exp(_lp);
	double xi = rng->exponential() / lambda;

	//Time of y'th jump
	if (_y == 0) {
	    _tau2 = 0;
	}
	else {
	    _tau2 = rbeta(_y, 1, rng);
	    _mix2->update(-log(_tau2) - _lp, _y, rng);
	}
	
	//Inter-arrival time to (y+1)'th jump
	_tau1 = 1 - _tau2 + xi;
	_mix1->update(-log(_tau1) - _lp, 1, rng);
    }

    double AuxMixPoisson::precision() const
    {
	if (_y == 0) {
	    return _mix1->precision();
	}
	else {
	    return _mix1->precision() + _mix2->precision();
	}
    }

    double AuxMixPoisson::value() const
    {
	if (_y == 0) {
	    return -(_mix1->mean() + log(_tau1));
	}
	else {
	    double w1 = _mix1->precision();
	    double y1 = -(_mix1->mean() + log(_tau1));
	
	    double w2 = _mix2->precision();
	    double y2 = -(_mix2->mean() + log(_tau2));

	    return (y1*w1 + y2*w2)/(w1 + w2);
	}
    }

    bool AuxMixPoisson::canRepresent(StochasticNode const *snode) 
    {
	return getFamily(snode) == GLM_POISSON && getLink(snode) == LNK_LOG;
    }

}}

