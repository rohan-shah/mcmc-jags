#include <config.h>

#include "HolmesHeldFactory.h"
#include "HolmesHeld.h"
#include "HolmesHeldGibbs.h"

using std::vector;

namespace jags {
namespace glm {

    HolmesHeldFactory::HolmesHeldFactory()
	: BinaryFactory("glm::Holmes-Held")
    {}

    GLMMethod *
    HolmesHeldFactory::newBinary(GraphView const *view,
				 vector<SingletonGraphView const *> const &sv,
				 vector<Outcome *> const &outcomes,
				 unsigned int chain, bool gibbs) const
    {
	if (gibbs) {
	    return new HolmesHeldGibbs(view, sv, outcomes, chain);
	}
	else {
	    return new HolmesHeld(view, sv, outcomes, chain);
	}
    }

}}
