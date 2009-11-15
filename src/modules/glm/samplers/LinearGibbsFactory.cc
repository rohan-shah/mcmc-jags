#include <config.h>

#include "LinearGibbsFactory.h"
#include "Linear.h"

#include <graph/StochasticNode.h>
#include <distribution/Distribution.h>

using std::vector;

namespace glm {

    LinearGibbsFactory::LinearGibbsFactory()
	: GLMFactory("LinearGibbs")
    {
    }
    
    bool LinearGibbsFactory::checkOutcome(StochasticNode const *snode,
					  LinkNode const *lnode) const
    {
	return snode->distribution()->name() == "dnorm" && lnode == 0;
    }
    
    GLMMethod*
    LinearGibbsFactory::newMethod(Updater const *updater,
			     vector<Updater const *> const &sub_updaters,
			     unsigned int chain) const
    {
	return new Linear(updater, sub_updaters, chain, true);
    }

    bool LinearGibbsFactory::canSample(StochasticNode const *snode) const
    {
	return snode->length() == 1;
    }
}
