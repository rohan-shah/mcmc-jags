#include <config.h>

#include "LinearGibbsFactory.h"
#include "Linear.h"

#include <graph/StochasticNode.h>
#include <distribution/Distribution.h>

using std::vector;

namespace glm {

    LinearGibbsFactory::LinearGibbsFactory()
	: GLMFactory("glm::LinearGibbs")
    {
    }
    
    bool LinearGibbsFactory::checkOutcome(StochasticNode const *snode,
					  LinkNode const *lnode) const
    {
	return snode->distribution()->name() == "dnorm" && lnode == 0;
    }
    
    GLMMethod*
    LinearGibbsFactory::newMethod(GraphView const *view,
			     vector<GraphView const *> const &sub_views,
			     unsigned int chain) const
    {
	return new Linear(view, sub_views, chain, true);
    }

    bool LinearGibbsFactory::canSample(StochasticNode const *snode) const
    {
	return snode->length() == 1;
    }
}
