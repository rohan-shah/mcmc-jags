#include <config.h>

#include "AlbertChibGibbsFactory.h"
#include "AlbertChib.h"

using std::vector;

namespace jags {
namespace glm {

    AlbertChibGibbsFactory::AlbertChibGibbsFactory()
	: BinaryFactory("glm::Albert-Chib-Gibbs")
    {}

    GLMMethod *
    AlbertChibGibbsFactory::newBinary(GraphView const *view,
				      vector<SingletonGraphView const *> const &sub_views,
				      vector<Outcome *> const &outcomes,
				      unsigned int chain) const
    {
	return new AlbertChib(view, sub_views, outcomes, chain, true);
    }

}}
