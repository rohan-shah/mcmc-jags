#include <config.h>

#include "AlbertChib.h"
#include "KS.h"
#include "Outcome.h"

#include <graph/StochasticNode.h>
#include <graph/LinkNode.h>
#include <sampler/GraphView.h>
#include <sampler/SingletonGraphView.h>
#include <rng/TruncatedNormal.h>
#include <rng/RNG.h>
#include <cmath>
#include <module/ModuleError.h>

using std::vector;

namespace jags {
namespace glm {

    AlbertChib::AlbertChib(GraphView const *view,
			   vector<SingletonGraphView const *> const &sub_views,
			   vector<Outcome *> const &outcomes,
			   unsigned int chain)
	: GLMBlock(view, sub_views, outcomes, chain)
    {
    }
	
}}

    
