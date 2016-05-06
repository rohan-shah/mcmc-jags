#include <config.h>

#include "HolmesHeldGibbs.h"
#include "KS.h"
#include "Outcome.h"

#include <graph/StochasticNode.h>
#include <graph/LinkNode.h>
#include <sampler/GraphView.h>
#include <rng/TruncatedNormal.h>
#include <rng/RNG.h>
#include <module/ModuleError.h>

#include <cmath>

using std::vector;

namespace jags {
    namespace glm {
	
	HolmesHeldGibbs::HolmesHeldGibbs(GraphView const *view,
		 vector<SingletonGraphView const *> const &sv,
		 vector<Outcome *> const &outcomes,
		 unsigned int chain)
	    : GLMGibbs(view, sv, outcomes, chain)
	{
	}

    }
}
    
