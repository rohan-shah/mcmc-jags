#include <config.h>
#include <distribution/Distribution.h>
//asStochastic
#include <graph/StochasticNode.h>
#include <graph/Graph.h>
#include <graph/NodeError.h>
#include <sampler/DensitySampler.h>

#include "DSumFactory.h"
#include "DSumSampler.h"

#include <stdexcept>
#include <algorithm>

using std::set;
using std::vector;
using std::runtime_error;

Sampler * DSumFactory::makeSampler(set<StochasticNode*> const &nodes,
				   Graph const &graph) const
{
    set<StochasticNode*> dsum_nodes;
    set<StochasticNode*>::const_iterator p;

    //Find DSum node
    for (p = nodes.begin(); p != nodes.end(); ++p) {
	if ((*p)->distribution()->name() == "dsum")
	    break;
    }
  
    if (p == nodes.end())
	return 0;

    
    //See if we can sample the parents. This can only be done if they
    //are unobserved stochastic nodes in the sample set

    vector<StochasticNode *> parameters;
    vector<Node const *> const &parents = (*p)->parents();
    vector<Node const *>::const_iterator pp;
    for (pp = parents.begin(); pp != parents.end(); ++pp) {
	set<StochasticNode *>::const_iterator q =
	    find(nodes.begin(), nodes.end(), *pp);
	if (q != nodes.end()) {
	    parameters.push_back(*q);
	}
	else {
	    return 0;
	}
    }
    
    if (!DSumMethod::canSample(parameters, graph)) {
	return 0;
    }
	
    unsigned int nchain = parameters[0]->nchain();
    vector<DensityMethod*> methods(nchain, 0);
    for (unsigned int ch = 0; ch < nchain; ++ch) {
	methods[ch] = new DSumMethod;
    }
    return new DensitySampler(parameters, graph, methods);

}
