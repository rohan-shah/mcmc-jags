#include <config.h>

#include "HolmesHeldBFactory.h"
#include "HolmesHeldB.h"

using std::vector;

namespace glm {

    HolmesHeldBFactory::HolmesHeldBFactory()
	: BinaryFactory("glm::Holmes-HeldB", false)
    {}

    BinaryGLM *
    HolmesHeldBFactory::newBinary(GraphView const *view,
				 vector<GraphView const *> const &sub_views,
				 unsigned int chain) const
    {
	return new HolmesHeldB(view, sub_views, chain);
    }

}
