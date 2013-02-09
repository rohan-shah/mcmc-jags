#include <config.h>

#include "IWLSOutcome.h"
#include "Classify.h"

#include <graph/LinkNode.h>
#include <graph/StochasticNode.h>
#include <module/ModuleError.h>

namespace jags {
namespace glm {

    /*
     * Work-around for the binomial distribution where the outcome of
     * Y ~ dbin(p, N) is Y/N
     */
    static double const &getScale(StochasticNode const *snode, unsigned int chain)
    {
	static const double one = 1;
	
	if (getFamily(snode) == GLM_BINOMIAL) {
	    Node const *N = snode->parents()[1];
	    return N->value(chain)[0];
	}
	else {
	    return one;
	}
    }

    IWLSOutcome::IWLSOutcome(StochasticNode const *snode, unsigned int chain)
	: Outcome(snode, chain), _link(dynamic_cast<LinkNode const*>(snode->parents()[0])), _family(getFamily(snode)), 
	  _y(snode->value(chain)[0]), _scale(getScale(snode, chain)), _chain(chain)
    {
    }
    
    double IWLSOutcome::precision() const
    {
	double grad = _link->grad(_chain);

	return (_scale * grad * grad)/ var();
    }

    double IWLSOutcome::value() const
    {
	double mu = _link->value(_chain)[0];
	double grad = _link->grad(_chain);
	
	return _lp + (_y / _scale - mu) / grad;
    }
 
    double IWLSOutcome::var() const
    {
	double mu = _link->value(_chain)[0];

	//FIXME, should be sub-classes
	switch(_family) {
	case GLM_BERNOULLI: case GLM_BINOMIAL:
	    return  mu * (1 - mu);
	    break;
	case GLM_POISSON:
	    return mu;
	    break;
	default:
	    throwLogicError("Invalid GLM family in IWLS");
	}

	return 0; //-Wall
    }

    bool IWLSOutcome::canRepresent(StochasticNode const *snode)
    {
	GLMFamily family = getFamily(snode);
	GLMLink link = getLink(snode);

	switch(family) {
	case GLM_BINOMIAL: case GLM_BERNOULLI:
	    return link == LNK_PROBIT || link == LNK_LOGIT;
	case GLM_POISSON:
	    return link == LNK_LOG;
	default:
	    break;
	}

	return false;
	
    }

}}
