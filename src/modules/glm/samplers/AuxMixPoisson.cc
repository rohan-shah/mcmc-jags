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
	//Time of y'th jump
	if (_y == 0) {
	    _tau2 = 0;
	}
	else {
	    _tau2 = rbeta(_y, 1, rng);
	    _mix2->update(-log(_tau2) - _lp, _y, rng);
	}
	
	//Inter-arrival time to (y+1)'th jump
	double xi = rng->exponential() / exp(_lp);
	_tau1 = 1 - _tau2 + xi;
	_mix1->update(-log(_tau1) - _lp, 1, rng);
    }

    /*
    void AuxMixPoisson::update(double mean, double var, RNG *rng)
    {
	double mu1 = _mix1->mean();
	double var1 =  _mix1->variance());
	
	//Time of y'th jump
	if (_y == 0) {
	    _tau2 = 0;
	    double nl_tau1 = rnormal(0, rng, mean + mu1, sqrt(var + var1));
	    _tau1 = exp(-nl_tau1);
	}
	else {


	       Somewhat complicated Gibbs update: we use the parameterization
	       -log(taui) = lp + ri; for i = 1,2 
	       where lp is the linear predictor, and ri is the residual, 
	       with mean and variance given by _mixi.

	       Components lp, r1, r2 have independent normal priors (conditional
	       on current mixture approximation) but are subject to constraints
	       tau1 <= 1 and tau1 + tau2 > 1.


	    double r1 = -log(_tau1) - _lp;
	    double r2 = -log(_tau2) - _lp;

	    // Update lp given r1, r2
	    double lp_lower = -r1;
	    double lp_upper = log(exp(-r1) + exp(-r2));
	    double lp = inormal(lp_lower, lp_upper, rng, mean, var);
	    
	    double mu2 = _mix2->mean();
	    double var2 = _mix2->variance();

	    // Update _tau2 given lp, _tau1
	    _tau1 = exp(-r1 - lp);
	    if (_tau1 < 1) {
		_tau2 = exp(-inormal(0, -log(1 - _tau1), rng,  lp + mu2, sqrt(var2)));
	    }
	    else {
		_tau2 = exp(-lnormal(0, rng, lp + mu2, sqrt(var2)));
	    }

	    // Update _tau1 given lp, _tau2
	    _tau1 = exp(-rnormal(-log(1 - _tau2), rng, lp + mu1, sqrt(var1)));
	}
    }
    */
    
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

