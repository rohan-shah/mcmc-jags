#include <config.h>

#include "AlbertChibGibbsFactory.h"
#include "AlbertChib.h"

using std::vector;

namespace glm {

    AlbertChibGibbsFactory::AlbertChibGibbsFactory()
	: BinaryFactory("glm::Albert-Chib-Gibbs", true)
    {}

    BinaryGLM *
    AlbertChibGibbsFactory::newBinary(GraphView const *view,
				 vector<GraphView const *> const &sub_views,
				 unsigned int chain) const
    {
	return new AlbertChib(view, sub_views, chain, true);
    }

}
