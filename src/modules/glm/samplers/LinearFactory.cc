#include <config.h>

#include "LinearFactory.h"
#include "GLMGibbs.h"
#include "GLMBlock.h"
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
	if (gibbs) {
	    return new GLMGibbs(view, subviews, outcomes, chain);
	}
	else {
	    return new GLMBlock(view, subviews, outcomes, chain);
	}
    }

}}
