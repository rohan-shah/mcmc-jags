#include <config.h>

#include <string>

#include "BinaryFactory.h"
#include "Linear.h"

#include "NormalLinear.h"
#include "BinaryProbit.h"
#include "BinaryLogit.h"

#include <graph/StochasticNode.h>
#include <graph/LinkNode.h>
#include <distribution/Distribution.h>
#include <module/ModuleError.h>

using std::string;
using std::vector;

namespace jags {
namespace glm {

    BinaryFactory::BinaryFactory(string const &name, bool gibbs)
	: GLMFactory(name), _gibbs(gibbs)
    {}

    bool BinaryFactory::checkOutcome(StochasticNode const *snode) const
    {
	return (BinaryProbit::canRepresent(snode) ||
		BinaryLogit::canRepresent(snode) ||
		NormalLinear::canRepresent(snode));
    }

    GLMMethod *
    BinaryFactory::newMethod(GraphView const *view,
			     vector<GraphView const *> const &sub_views,
			     unsigned int chain) const
    {
	bool linear = true;
	vector<Outcome*> outcomes;

	for (vector<StochasticNode const*>::const_iterator p = view->stochasticChildren().begin();
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
	    return new Linear(view, sub_views, outcomes, chain, _gibbs);
	}
	else {
	    return newBinary(view, sub_views, outcomes, chain);
	}
    }

    bool BinaryFactory::canSample(StochasticNode const *snode) const
    {
	if (_gibbs) {
	    return snode->length() == 1;
	}
	else {
	    return !isBounded(snode);
	}
    }

}}
