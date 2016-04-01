#include <config.h>

#include "LinearFactory.h"
#include "Linear.h"
#include "NormalLinear.h"

#include <graph/StochasticNode.h>
#include <distribution/Distribution.h>
#include <sampler/SingletonGraphView.h>

using std::vector;

namespace jags {
namespace glm {

    LinearFactory::LinearFactory()
	: GLMFactory("glm::Linear")
    {
    }
    
    bool LinearFactory::checkOutcome(StochasticNode const *snode) const
    {
	return NormalLinear::canRepresent(snode);
    }
    
    GLMMethod*
    LinearFactory::newMethod(GraphView const *view,
			     vector<SingletonGraphView const *> const &subviews,
			     unsigned int chain, bool gibbs) const
    {
	vector<Outcome*> outcomes;
	vector<StochasticNode *>::const_iterator p;
	for (p = view->stochasticChildren().begin();
	     p != view->stochasticChildren().end(); ++p)
	{
	    outcomes.push_back(new NormalLinear(*p, chain));
	}
	return new Linear(view, subviews, outcomes, chain, gibbs);
    }

}}
