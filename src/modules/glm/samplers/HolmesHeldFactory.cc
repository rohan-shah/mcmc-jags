#include <config.h>

#include <string>

#include "HolmesHeldFactory.h"
#include "HolmesHeld.h"
#include "ConjugateLM.h"

#include <graph/StochasticNode.h>
#include <graph/LinkNode.h>
#include <distribution/Distribution.h>
#include <function/InverseLinkFunc.h>

using std::string;
using std::vector;

namespace glm {

    HolmesHeldFactory::HolmesHeldFactory()
	: GLMFactory("Holmes-Held")
    {}

    bool HolmesHeldFactory::checkOutcome(StochasticNode const *snode,
					 LinkNode const *lnode) const
    {
	Node const *N = 0;
	string linkname;
	if (lnode) {
	    linkname = lnode->link()->linkName();
	}

	switch(GLMMethod::getFamily(snode)) {
	case GLM_BERNOULLI:
	    return linkname == "probit" || linkname=="logit";
	case GLM_BINOMIAL:
	    N = snode->parents()[1];
	    if (N->length() != 1)
		return false;
	    if (!N->isObserved())
		return false;
	    if (N->value(0)[0] != 1)
		return false;
	    return linkname == "probit" || linkname=="logit";
	case GLM_NORMAL:
	    return lnode == 0;
	default:
	    return false;
	}
    }
    
    GLMMethod *
    HolmesHeldFactory::newMethod(Updater const *updater,
			     vector<Updater const *> const &sub_updaters,
			     unsigned int chain) const
    {
	/* 
	   If we have a pure guassian linear model then make a
	   conjugate linear sampler instead. There is no need, in this
	   case, for the extra machinery of the Holmes-Held sampler.
	*/
	bool linear = true;
	vector<StochasticNode const*> const &children =
	    updater->stochasticChildren();
	for (unsigned int i = 0; i < children.size(); ++i) {
	    if (GLMMethod::getFamily(children[i]) != GLM_NORMAL) {
		linear = false;
		break;
	    }
	}

	if (linear) {
	    return new ConjugateLM(updater, sub_updaters, chain);
	}
	else {
	    return new HolmesHeld(updater, sub_updaters, chain);
	}
    }

}
