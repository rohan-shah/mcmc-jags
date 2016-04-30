#include <config.h>

#include "AlbertChibFactory.h"
#include "GLMGibbs.h"
#include "GLMBlock.h"

#include <graph/StochasticNode.h>

using std::vector;

namespace jags {
namespace glm {

    AlbertChibFactory::AlbertChibFactory()
	: BinaryFactory("glm::Albert-Chib")
    {}

    GLMMethod *
    AlbertChibFactory::newBinary(GraphView const *view,
				 vector<SingletonGraphView const *> const &sub_views,
				 vector<Outcome *> const &outcomes,
				 unsigned int chain, bool gibbs) const
    {

	if (gibbs) {
	    return new GLMGibbs(view, sub_views, outcomes, chain);
	}
	else {
	    return new GLMBlock(view, sub_views, outcomes, chain);
	}
	    
    }

}}
