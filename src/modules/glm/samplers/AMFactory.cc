#include <config.h>

#include <string>

#include "BinaryFactory.h"
#include "AMFactory.h"
#include "AMMethod.h"
#include "Linear.h"

#include <graph/StochasticNode.h>
#include <graph/LinkNode.h>
#include <distribution/Distribution.h>
#include <function/InverseLinkFunc.h>

using std::string;
using std::vector;

namespace glm {

    AMFactory::AMFactory()
	: GLMFactory("Auxiliary-Mixture")
    {}

    bool AMFactory::checkOutcome(StochasticNode const *snode,
				 LinkNode const *lnode) const
    {
	string linkname;
	if (lnode) {
	    linkname = lnode->link()->linkName();
	}

	switch(GLMMethod::getFamily(snode)) {
	case GLM_BERNOULLI: case GLM_BINOMIAL:
	    return linkname=="logit";
	case GLM_POISSON:
	    return linkname=="log";
	    /*
	      case GLM_NORMAL:
	      return lnode == 0;
	    */
	default:
	    return false;
	}
    }
    
    GLMMethod *
    AMFactory::newMethod(Updater const *updater,
			     vector<Updater const *> const &sub_updaters,
			     unsigned int chain) const
    {
	/* 
	   If we have a pure guassian linear model then make a
	   conjugate linear sampler instead. 
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
	    return new Linear(updater, sub_updaters, chain, false);
	}
	else {
	    return new AMMethod(updater, sub_updaters, chain);
	}
    }

    bool AMFactory::canSample(StochasticNode const *snode) const
    {
	return !isBounded(snode);
    }
}
