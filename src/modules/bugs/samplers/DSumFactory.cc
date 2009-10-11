#include <config.h>
#include <distribution/Distribution.h>
//asStochastic
#include <graph/StochasticNode.h>
#include <graph/Graph.h>
#include <graph/NodeError.h>
#include <sampler/ParallelSampler.h>
#include <sampler/Updater.h>

#include "DSumFactory.h"
#include "DSumMethod.h"

#include <stdexcept>
#include <algorithm>

using std::set;
using std::vector;
using std::runtime_error;
using std::string;

static StochasticNode const *getDSumChild(StochasticNode *node)
{
    //We can skip nodes that are not scalar discrete-valued
    //as they cannot be sampled by a DSumSampler
    if (node->length() != 1 || !node->isDiscreteValued())
	return 0;
    
    set<StochasticNode*>::const_iterator p;
    for (p = node->stochasticChildren()->begin(); 
	 p != node->stochasticChildren()->end(); ++p) 
    {
	//Skip unobserved nodes
	if ((*p)->isObserved() && (*p)->distribution()->name() == "dsum") 
	    return *p;
    }
    return 0;
}

DSumFactory::DSumFactory()
    : _name("DSum")
{}


Sampler * DSumFactory::makeSampler(set<StochasticNode*> const &nodes,
				   Graph const &graph) const
{
    //Find DSum node
    StochasticNode const *dsum_node = 0;
    for (set<StochasticNode*>::const_iterator p = nodes.begin(); 
	 p != nodes.end(); ++p) 
    {
	dsum_node = getDSumChild(*p);
	if (dsum_node)
	    break;
    }
    
    if (!dsum_node)
	return 0;
    
    //See if we can sample the parents. This can only be done if they
    //are unobserved stochastic nodes in the sample set

    vector<StochasticNode *> parameters;
    vector<Node const *> const &parents = dsum_node->parents();
    vector<Node const *>::const_iterator pp;
    for (pp = parents.begin(); pp != parents.end(); ++pp) {
	set<StochasticNode*>::const_iterator q =
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

    Updater *updater = new Updater(parameters, graph);
    unsigned int nchain = parameters[0]->nchain();
    vector<SampleMethod*> methods(nchain, 0);
    for (unsigned int ch = 0; ch < nchain; ++ch) {
	methods[ch] = new DSumMethod(updater, ch);
    }
    return new ParallelSampler(updater, methods);

}

string const &DSumFactory::name() const
{
    return _name;
}
