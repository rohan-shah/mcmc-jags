#include <config.h>

#include <string>

#include "BinaryFactory.h"

#include "NormalLinear.h"
#include "BinaryProbit.h"
#include "BinaryLogit.h"

#include "GLMGibbs.h"
#include "GLMBlock.h"

#include <graph/StochasticNode.h>
#include <graph/LinkNode.h>
#include <distribution/Distribution.h>
#include <module/ModuleError.h>
#include <sampler/SingletonGraphView.h>

using std::string;
using std::vector;

namespace jags {
namespace glm {

    BinaryFactory::BinaryFactory(string const &name)
	: GLMFactory(name)
    {}

    bool BinaryFactory::checkOutcome(StochasticNode const *snode) const
    {
	return (BinaryProbit::canRepresent(snode) ||
		BinaryLogit::canRepresent(snode) ||
		NormalLinear::canRepresent(snode));
    }

    GLMMethod *
    BinaryFactory::newMethod(GraphView const *view,
			     vector<SingletonGraphView const *> const &subviews,
			     unsigned int chain, bool gibbs) const
    {
	bool linear = true;
	vector<Outcome*> outcomes;

	vector<StochasticNode *>::const_iterator p;
	for (p = view->stochasticChildren().begin();
	     p != view->stochasticChildren().end(); ++p)
	{
	    Outcome *outcome = 0;
	    if (NormalLinear::canRepresent(*p)) {
		outcome = new NormalLinear(*p, chain);
	    }
	    else if (BinaryProbit::canRepresent(*p)) {
		outcome = new BinaryProbit(*p, chain);
		linear = false;
	    }
	    else if (BinaryLogit::canRepresent(*p)) {
		outcome = new BinaryLogit(*p, chain);
		linear = false;
	    }
	    else {
		throwLogicError("Invalid outcome in BinaryFactory");
	    }
	    outcomes.push_back(outcome);
	}

	if (linear) {
	    if (gibbs) {
		return new GLMGibbs(view, subviews, outcomes, chain);
	    }
	    else {
		return new GLMBlock(view, subviews, outcomes, chain);
	    }
	}
	else {
	    return newBinary(view, subviews, outcomes, chain, gibbs);
	}
    }

}}
