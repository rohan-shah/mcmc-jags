#include <config.h>

#include "BinaryGLM.h"

#include <graph/StochasticNode.h>
#include <graph/LinkNode.h>
#include <function/InverseLinkFunc.h>
#include <sampler/Updater.h>
#include <rng/TruncatedNormal.h>
#include <rng/RNG.h>

#include <stdexcept>

using std::vector;
using std::string;
using std::logic_error;

#define CHILD(i) (_updater->stochasticChildren()[i])

static unsigned int nchildren(Updater const *updater)
{
    return updater->stochasticChildren().size();
}

static BGLMOutcome getOutcome(StochasticNode const *snode)
{
    LinkNode const *lnode = 0;

    switch(glm::GLMMethod::getFamily(snode)) {
    case GLM_NORMAL:
	return BGLM_NORMAL;
    case GLM_BERNOULLI: case GLM_BINOMIAL:
	lnode = dynamic_cast<LinkNode const*>(snode->parents()[0]);
	if (!lnode) {
	    throw logic_error("No link in Holmesheld");
	}
	else if (lnode->link()->linkName() == "probit") {
	    return BGLM_PROBIT;
	}
	else if (lnode->link()->linkName() == "logit") {
	    return BGLM_LOGIT;
	}
	else {
	    throw logic_error("Invalid link in BinaryGLM");
	}
    default:
	throw logic_error("Invalid family in BinaryGLM");
    }
}

namespace glm {

    BinaryGLM::BinaryGLM(Updater const *updater,
			   vector<Updater const *> const &sub_updaters,
			   unsigned int chain)
	: GLMMethod(updater, sub_updaters, chain, true), 
	  _outcome(nchildren(updater)),
	  _z(nchildren(updater)), _tau(nchildren(updater))
    {
	for (unsigned int i = 0; i < _outcome.size(); ++i) {
	    _outcome[i] = getOutcome(CHILD(i));
	}
    }
    
    double BinaryGLM::getValue(unsigned int i) const 
    {
	double z = 0;
	
	switch(_outcome[i]) {
	case BGLM_NORMAL:
	    z = CHILD(i)->value(_chain)[0];
	    break;
	case BGLM_PROBIT: case BGLM_LOGIT:
	    z = _z[i];
	    break;
	}

	return z;
    }

    double BinaryGLM::getPrecision(unsigned int i) const
    {
	double tau = 0;
	switch(_outcome[i]) {
	case BGLM_NORMAL:
	    tau = CHILD(i)->parents()[1]->value(_chain)[0];
	    break;
	case BGLM_PROBIT:
	    tau = 1;
	    break;
	case BGLM_LOGIT:
	    tau = _tau[i];
	    break;
	}
	
	return tau;
    }


    void BinaryGLM::initAuxiliary(RNG *rng)    
    {	    
	for (unsigned int i = 0; i < _z.size(); ++i) {
	    _tau[i] = 1;
	    double y = CHILD(i)->value(_chain)[0];
	    switch(_outcome[i]) {
	    case BGLM_NORMAL:
		_z[i] = 0;
		break;
	    case BGLM_PROBIT: case BGLM_LOGIT:
		if (y == 1) {
		    _z[i] = lnormal(0, rng, getMean(i));
		}
		else if (y == 0) {
		    _z[i] = rnormal(0, rng, getMean(i));
		}
		else {
		    throw logic_error("Invalid child value in BinaryGLM");
		}
	    }
	}

    }

}

    
