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
using std::string;
using std::log;
using std::exp;
using std::fabs;

namespace jags {
namespace glm {

    AlbertChib::AlbertChib(GraphView const *view,
			   vector<SingletonGraphView const *> const &sub_views,
			   vector<Outcome *> const &outcomes,
			   unsigned int chain, bool gibbs)
	: GLMMethod(view, sub_views, outcomes, chain, true), _gibbs(gibbs)
    {
    }

    void AlbertChib::update(RNG *rng)
    {
	/*
	  Note that we must update the auxiliary variables *before*
	  calling updateLM. This ordering is important for models with
	  a variable design matrix (e.g. measurement error models).
	*/

	for (vector<Outcome*>::const_iterator p = _outcomes.begin();
	     p != _outcomes.end(); ++p)
	{
	    (*p)->update(rng);
	}

	if (_gibbs) {
	    updateLMGibbs(rng);
	}
	else {
	    updateLM(rng);
	}
    }
	
    string AlbertChib::name() const
    {
	if (_gibbs)
	    return "Albert-Chib-Gibbs";
	else
	    return "Albert-Chib";
    }

}}

    
