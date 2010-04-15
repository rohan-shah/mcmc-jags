#include <config.h>

#include "LinearFactory.h"
#include "Linear.h"

#include <graph/StochasticNode.h>
#include <distribution/Distribution.h>

using std::vector;

namespace glm {

    LinearFactory::LinearFactory()
	: GLMFactory("glm::Linear")
    {
    }
    
    bool LinearFactory::checkOutcome(StochasticNode const *snode,
					  LinkNode const *lnode) const
    {
	return snode->distribution()->name() == "dnorm" && lnode == 0;
    }
    
    GLMMethod*
    LinearFactory::newMethod(GraphView const *view,
			     vector<GraphView const *> const &sub_views,
			     unsigned int chain) const
    {
	return new Linear(view, sub_views, chain, false);
    }

    bool LinearFactory::canSample(StochasticNode const *snode) const
    {
	return !isBounded(snode);
    }
}
