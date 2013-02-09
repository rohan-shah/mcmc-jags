#include <config.h>

#include "LinearGibbsFactory.h"
#include "Linear.h"
#include "NormalLinear.h"

#include <graph/StochasticNode.h>
#include <distribution/Distribution.h>

using std::vector;

namespace jags {
namespace glm {

    LinearGibbsFactory::LinearGibbsFactory()
	: GLMFactory("glm::LinearGibbs")
    {
    }
    
    bool LinearGibbsFactory::checkOutcome(StochasticNode const *snode) const
    {
        return NormalLinear::canRepresent(snode);
    }
    
    GLMMethod*
    LinearGibbsFactory::newMethod(GraphView const *view,
			     vector<GraphView const *> const &sub_views,
			     unsigned int chain) const
    {
	vector<Outcome*> outcomes;
	for (vector<StochasticNode const*>::const_iterator p = view->stochasticChildren().begin();
	     p != view->stochasticChildren().end(); ++p)
	{
	    outcomes.push_back(new NormalLinear(*p, chain));
	}

	return new Linear(view, sub_views, outcomes, chain, true);
    }

    bool LinearGibbsFactory::canSample(StochasticNode const *snode) const
    {
	return snode->length() == 1;
    }

}}
