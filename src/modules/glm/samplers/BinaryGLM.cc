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

namespace jags {

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
			 vector<Outcome*> const &outcomes,
			 unsigned int chain)
	: GLMMethod(view, sub_views, outcomes, chain, true)
    {
    }
    

}}

    
