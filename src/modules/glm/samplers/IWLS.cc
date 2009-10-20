#include <config.h>
#include "IWLS.h"
#include <graph/LinkNode.h>
#include <graph/StochasticNode.h>
#include <function/InverseLinkFunc.h>
#include <distribution/Distribution.h>

#include <stdexcept>

using std::string;
using std::vector;
using std::logic_error;

static unsigned int nchildren(Updater const *updater)
{
    return updater->stochasticChildren().size();
}

namespace glm {


    IWLS::IWLS(Updater const *updater, 
	       vector<Updater const *> const &sub_updaters,
	       unsigned int chain)
	: GLMMethod(updater, sub_updaters, chain, true),
	  _link(nchildren(updater)), _family(nchildren(updater))
    {
	vector<StochasticNode const*> const &children =
	    updater->stochasticChildren();
	
	for (unsigned int i = 0; i < children.size(); ++i) {
	    _link[i] = dynamic_cast<LinkNode const*>(children[i]->parents()[0]);
	    _family[i] = getFamily(children[i]);
	}
    }
    
    string IWLS::name() const
    {
	return "iwls";
    }

    double IWLS::getMean(unsigned int i) const
    {
	return _link[i]->value(_chain)[0];
    }

    double IWLS::getPrecision(unsigned int i) const
    {
	double grad = _link[i]->grad(_chain);
	return getCurve(i) * grad * grad;
    }

    double IWLS::getValue(unsigned int i) const
    {
	double y = _updater->stochasticChildren()[i]->value(_chain)[0];
	double mu = getMean(i);
	double eta = _link[i]->eta(_chain);
	double grad = _link[i]->grad(_chain);
	
	return eta + (y - mu) * grad;
    }
	  
    double IWLS::getCurve(unsigned int i) const
    {
	StochasticNode const *child = _updater->stochasticChildren()[i];
	vector<Node const*> const &params = child->parents();
    
	double mu, phi;

	switch(_family[i]) {
	case GLM_BERNOULLI:
	    mu = params[0]->value(_chain)[0];
	    return  mu * (1 - mu);
	    break;
	case GLM_BINOMIAL:
	    mu = params[0]->value(_chain)[0];
	    phi = params[1]->value(_chain)[0];
	    return phi * mu * (1 - mu);
	    break;
	case GLM_UNKNOWN:
	    throw logic_error("Unknown GLM family in IWLS");
	}
    }
 
    GLMFamily IWLS::getFamily(StochasticNode const *snode)
    {
	string const &name = snode->distribution()->name();
	if (name == "dbern") {
	    return GLM_BERNOULLI;
	}
	else if (name == "dbinom") {
	    return GLM_BINOMIAL;
	}
	else {
	    return GLM_UNKNOWN;
	}
    }

    void IWLS::initAuxiliary(RNG *rng)
    {
	//No auxiliary variables in this method
    }
    
    void IWLS::updateAuxiliary(double *b, csn const *N, RNG *rng)
    {
	//No auxiliary variables in this method
    }
}
