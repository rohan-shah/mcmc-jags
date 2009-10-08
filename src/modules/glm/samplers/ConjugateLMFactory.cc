#include <config.h>

#include "ConjugateLMFactory.h"
#include "ConjugateLM.h"

#include <graph/StochasticNode.h>
#include <distribution/Distribution.h>

using std::vector;

namespace glm {

    bool ConjugateLMFactory::checkOutcome(StochasticNode const *snode) const
    {
	return snode->distribution()->name() == "dnorm";
    }
    
    bool ConjugateLMFactory::checkLink(InverseLinkFunc const *link) const
    {
	return false;
    }

    GLMMethod*
    ConjugateLMFactory::newMethod(Updater const *updater,
				  vector<Updater const *> const &sub_updaters,
				  unsigned int chain) const
    {
	return new ConjugateLM(updater, sub_updaters, chain);
    }

}
