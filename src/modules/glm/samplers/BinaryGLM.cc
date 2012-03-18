#include <config.h>

#include "BinaryGLM.h"

#include <graph/StochasticNode.h>
#include <graph/LinkNode.h>
#include <sampler/GraphView.h>
#include <rng/TruncatedNormal.h>
#include <rng/RNG.h>
#include <module/ModuleError.h>

using std::vector;
using std::string;

#define CHILD(i) (_view->stochasticChildren()[i])

static inline unsigned int nchildren(GraphView const *view)
{
    return view->stochasticChildren().size();
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
	    throwLogicError("No link in BinaryGLM");
	}
	else if (lnode->linkName() == "probit") {
	    return BGLM_PROBIT;
	}
	else if (lnode->linkName() == "logit") {
	    return BGLM_LOGIT;
	}
	else {
	    throwLogicError("Invalid link in BinaryGLM");
	}
	break;
    default:
	throwLogicError("Invalid family in BinaryGLM");
    }
    return BGLM_NORMAL; //-Wall
}

namespace glm {

    BinaryGLM::BinaryGLM(GraphView const *view,
			   vector<GraphView const *> const &sub_views,
			   unsigned int chain)
	: GLMMethod(view, sub_views, chain, true), 
	  _outcome(nchildren(view)),
	  _z(nchildren(view), 0), _tau(nchildren(view), 1)
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
	    double y = CHILD(i)->value(_chain)[0];
	    switch(_outcome[i]) {
	    case BGLM_PROBIT: case BGLM_LOGIT:
		if (y == 1) {
		    _z[i] = lnormal(0, rng, getMean(i));
		}
		else if (y == 0) {
		    _z[i] = rnormal(0, rng, getMean(i));
		}
		else {
		    throwLogicError("Invalid child value in BinaryGLM");
		}
		break;
	    case BGLM_NORMAL:
		break; //We don't use _z[i] for normal outcomes
	    }
	}
    }

}

    
