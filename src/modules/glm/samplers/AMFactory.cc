#include <config.h>

#include <string>

#include "BinaryFactory.h"
#include "AMFactory.h"
#include "AMMethod.h"
#include "Linear.h"

#include "AuxMixPoisson.h"
#include "AuxMixBinomial.h"
#include "NormalLinear.h"

#include <graph/StochasticNode.h>
#include <graph/LinkNode.h>
#include <distribution/Distribution.h>
#include <module/ModuleError.h>

using std::string;
using std::vector;

namespace jags {
namespace glm {

    AMFactory::AMFactory()
	: GLMFactory("glm::Auxiliary-Mixture")
    {}

    bool AMFactory::checkOutcome(StochasticNode const *snode) const
    {
	return AuxMixPoisson::canRepresent(snode) ||
	    AuxMixBinomial::canRepresent(snode) ||
	    NormalLinear::canRepresent(snode);
    }
    
    GLMMethod *
    AMFactory::newMethod(GraphView const *view,
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
	    else if (AuxMixBinomial::canRepresent(*p)) {
		outcome = new AuxMixBinomial(*p, chain);
		linear = false;
	    }
	    else if (AuxMixPoisson::canRepresent(*p)) {
		outcome = new AuxMixPoisson(*p, chain);
		linear = false;
	    }
	    else {
		throwLogicError("Invalid outcome in BinaryFactory");
	    }
	    outcomes.push_back(outcome);
	}

	if (linear) {
	    return new Linear(view, sub_views, outcomes, chain, false);
	}
	else {
	    return new AMMethod(view, sub_views, outcomes, chain);
	}
    }

    bool AMFactory::canSample(StochasticNode const *snode) const
    {
	return !isBounded(snode);
    }

}}
