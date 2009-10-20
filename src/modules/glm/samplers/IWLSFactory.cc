#include <config.h>

#include "IWLSFactory.h"
#include "IWLS.h"

using std::vector;

namespace glm {

    IWLSFactory::IWLSFactory()
	: GLMFactory("IWLS")
    {}

    bool IWLSFactory::checkOutcome(StochasticNode const *snode) const
    {
	return IWLS::getFamily(snode) != GLM_UNKNOWN;
    }
    
    bool IWLSFactory::checkLink(InverseLinkFunc const *link) const
    {
	return true;
    }

    GLMMethod *
    IWLSFactory::newMethod(Updater const *updater,
			     vector<Updater const *> const &sub_updaters,
			     unsigned int chain) const
    {
	return new IWLS(updater, sub_updaters, chain);
    }

}
