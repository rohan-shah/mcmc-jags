#include <config.h>

#include <string>

#include "BinaryFactory.h"
#include "AMFactory.h"
#include "AMMethod.h"
#include "Linear.h"

#include <graph/StochasticNode.h>
#include <graph/LinkNode.h>
#include <distribution/Distribution.h>

using std::string;
using std::vector;

namespace glm {

    AMFactory::AMFactory()
	: GLMFactory("glm::Auxiliary-Mixture")
    {}

    bool AMFactory::checkOutcome(StochasticNode const *snode,
				 LinkNode const *lnode) const
    {
	string linkname;
	if (lnode) {
	    linkname = lnode->linkName();
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
    AMFactory::newMethod(GraphView const *view,
			     vector<GraphView const *> const &sub_views,
			     unsigned int chain) const
    {
	/* 
	   If we have a pure guassian linear model then make a
	   conjugate linear sampler instead. 
	*/
	bool linear = true;
	vector<StochasticNode const*> const &children =
	    view->stochasticChildren();
	for (unsigned int i = 0; i < children.size(); ++i) {
	    if (GLMMethod::getFamily(children[i]) != GLM_NORMAL) {
		linear = false;
		break;
	    }
	}

	if (linear) {
	    return new Linear(view, sub_views, chain, false);
	}
	else {
	    return new AMMethod(view, sub_views, chain);
	}
    }

    bool AMFactory::canSample(StochasticNode const *snode) const
    {
	return !isBounded(snode);
    }

}
