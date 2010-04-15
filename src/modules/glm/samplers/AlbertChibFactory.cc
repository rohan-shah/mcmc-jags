#include <config.h>

#include "AlbertChibFactory.h"
#include "AlbertChib.h"
#include <graph/StochasticNode.h>

using std::vector;

namespace glm {

    AlbertChibFactory::AlbertChibFactory()
	: BinaryFactory("glm::Albert-Chib", false)
    {}

    BinaryGLM *
    AlbertChibFactory::newBinary(GraphView const *view,
				 vector<GraphView const *> const &sub_views,
				 unsigned int chain) const
    {
	return new AlbertChib(view, sub_views, chain, false);
    }

}
