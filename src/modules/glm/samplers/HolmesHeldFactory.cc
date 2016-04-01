#include <config.h>

#include "HolmesHeldFactory.h"
#include "HolmesHeld.h"

using std::vector;

namespace jags {
namespace glm {

    HolmesHeldFactory::HolmesHeldFactory()
	: BinaryFactory("glm::Holmes-Held")
    {}

    GLMMethod *
    HolmesHeldFactory::newBinary(GraphView const *view,
				 vector<SingletonGraphView const *> const &sub_views,
				 vector<Outcome *> const &outcomes,
				 unsigned int chain, bool gibbs) const
    {
	return new HolmesHeld(view, sub_views, outcomes, chain, gibbs);
    }

}}
