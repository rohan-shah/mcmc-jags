#include <config.h>

#include "Linear.h"
#include <graph/StochasticNode.h>
#include <sampler/SingletonGraphView.h>

using std::vector;
using std::string;

namespace jags {
namespace glm {

    Linear::Linear(GraphView const *view, 
		   vector<SingletonGraphView const *> const &sub_views,
		   vector<Outcome*> const &outcomes,
		   unsigned int chain, bool gibbs)
	: GLMMethod(view, sub_views, outcomes, chain), _gibbs(gibbs)
    {
    }

    void Linear::update(RNG *rng)
    {
	if (_gibbs) 
	    updateLMGibbs(rng);
	else
	    updateLM(rng);
    }

}}
