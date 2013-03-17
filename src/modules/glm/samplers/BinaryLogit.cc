#include "BinaryLogit.h"
#include "Classify.h"

#include "KS.h"

#include <graph/StochasticNode.h>
#include <rng/TruncatedNormal.h>
#include <rng/RNG.h>

#include <cmath>

using std::exp;
using std::log;

#define REG_PENALTY 0.001

//Left truncated logistic distribution
static double llogit(double left, jags::RNG *rng, double mu)
{
    double qleft = 1/(1 + exp(mu-left));
    double x = qleft + (1 - qleft) * rng->uniform();
    return mu + log(x) - log(1 - x);
}

//Right truncated logistic distribution
static double rlogit(double right, jags::RNG *rng, double mu)
{
    double qright = 1/(1 + exp(mu-right));
    double x = qright * rng->uniform();
    return mu + log(x) - log(1 - x);
}


double sample_lp(double y, double delta, double mean, double var, jags::RNG *rng)
{
    /* 
     * Helper function for marginal sampling. Samples the linear
     * predictor "mu" given prior mean, variance and posterior
     * constraint that "z = mu + delta < 0" or "z = mu + delta >= 0"
     * depending on the value of y.
     */
    if (y) {
	return lnormal(-delta, rng, mean, sqrt(var));
    }
    else {
	return rnormal(-delta, rng, mean, sqrt(var));
    }
}

double sample_delta(double y, double mu, jags::RNG *rng)
{
    /*
     * Helper function for marginal sampling. Samples the residual
     * "delta = z - mu" of the auxiliary variable, which has a prior
     * logistic distribution, given posterior constraint that "z < 0"
     * or "z >= 0", depending on the value of y
     */
    if (y) {
	return llogit(-mu, rng, 0); 
    }
    else {
	return rlogit(-mu, rng, 0); 
    }
}

namespace jags {
namespace glm {

    BinaryLogit::BinaryLogit(StochasticNode const *snode, unsigned int chain)
	: Outcome(snode, chain), _y(snode->value(chain)[0]), _z(0), _delta(0), _tau(1)
    {
	//fixme: sanity checks on snode
    }

    double BinaryLogit::value() const 
    {
	return _z;
    }

    double BinaryLogit::precision() const 
    {
	return _tau;
    }
    
    void BinaryLogit::update(RNG *rng)
    {
	/* Albert Chib update */
	if (_y) {
	    _z = llogit(0, rng, _lp);
	}
	else {
	    _z = rlogit(0, rng, _lp);
	}
	_tau = REG_PENALTY + 1/sample_lambda(_z - _lp, rng);

    }

    void BinaryLogit::update(double mean, double var, RNG *rng)
    {
	/* Holmes Held update */

	/* We generate a reversible transition for z using the
	   partitioning "z = lp + _delta". Instead of using the
	   current value of the linear predictor (_lp inherited from
	   Outcome) we sample a private copy. */
	   
	double lp = sample_lp(_y, _delta, mean, var, rng);
	_delta = sample_delta(_y, lp, rng);
	lp = sample_lp(_y, _delta, mean, var, rng);

	_z = lp + _delta;
	_tau = REG_PENALTY + 1/sample_lambda(_z - lp, rng);
    }

    bool BinaryLogit::canRepresent(StochasticNode const *snode)
    {
	//FIXME: Big overlap with BinaryProbit::canRepresent

	Node const *N = 0;

	switch(getFamily(snode)) {
	case GLM_BERNOULLI:
            break;
	case GLM_BINOMIAL:
            // We can also model binomial distribution if the denominator is fixed to be 1.                                                                                    
            N = snode->parents()[1];
            if (N->length() != 1)
		return false;
            if (!N->isFixed())
		return false;
            if (N->value(0)[0] != 1)
		return false;
            break;
	default:
            return false;
	}

	return getLink(snode) == LNK_LOGIT;
    }

}}
