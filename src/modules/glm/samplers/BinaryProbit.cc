#include "BinaryProbit.h"
#include "Classify.h"

#include <graph/StochasticNode.h>
#include <rng/TruncatedNormal.h>

#include <cmath>

using std::sqrt;

namespace jags {
namespace glm {

    BinaryProbit::BinaryProbit(StochasticNode const *snode, unsigned int chain)
	: Outcome(snode, chain), _y(snode->value(chain)[0]), _z(0)
    {
	//Fixme: sanity checks on snode
    }

    double BinaryProbit::value() const 
    {
	return _z;
    }

    double BinaryProbit::precision() const 
    {
	return 1;
    }
    
    void BinaryProbit::update(RNG *rng)
    {
	if (_y) {
	    _z = lnormal(0, rng, _lp, 1);
	}
	else {
	    _z = rnormal(0, rng, _lp, 1);
	}
    }

    void BinaryProbit::update(double mean, double var, RNG *rng)
    {
	if (_y) {
	    _z = lnormal(0, rng, mean, sqrt(var + 1));
	}
	else {
	    _z = rnormal(0, rng, mean, sqrt(var + 1));
	}
    }

    bool BinaryProbit::canRepresent(StochasticNode const *snode)
    {
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

	return getLink(snode) == LNK_PROBIT;
    }

    bool BinaryProbit::fixedA() const
    {
	return true;
    }

}}
