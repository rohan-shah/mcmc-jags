#include <config.h>

#include "ConjugateLMFactory.h"
#include "ConjugateLM.h"

#include <graph/StochasticNode.h>
#include <distribution/Distribution.h>

using std::vector;

namespace glm {

    ConjugateLMFactory::ConjugateLMFactory()
	: GLMFactory("ConjugateLM")
    {
    }
    
    bool ConjugateLMFactory::checkOutcome(StochasticNode const *snode,
					  LinkNode const *lnode) const
    {
	return snode->distribution()->name() == "dnorm" && lnode == 0;
    }
    
    GLMMethod*
    ConjugateLMFactory::newMethod(Updater const *updater,
				  vector<Updater const *> const &sub_updaters,
				  unsigned int chain) const
    {
	return new ConjugateLM(updater, sub_updaters, chain);
    }

}
