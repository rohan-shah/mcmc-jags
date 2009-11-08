#include <config.h>

#include "LinearFactory.h"
#include "Linear.h"

#include <graph/StochasticNode.h>
#include <distribution/Distribution.h>

using std::vector;

namespace glm {

    LinearFactory::LinearFactory()
	: GLMFactory("Linear")
    {
    }
    
    bool LinearFactory::checkOutcome(StochasticNode const *snode,
					  LinkNode const *lnode) const
    {
	return snode->distribution()->name() == "dnorm" && lnode == 0;
    }
    
    GLMMethod*
    LinearFactory::newMethod(Updater const *updater,
			     vector<Updater const *> const &sub_updaters,
			     unsigned int chain) const
    {
	return new Linear(updater, sub_updaters, chain, true);
    }

    bool LinearFactory::trunc() const
    {
	//return false;
	return true;
    }
}
