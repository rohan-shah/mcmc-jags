#include <config.h>
#include <distribution/Distribution.h>
//asStochastic
#include <graph/StochasticNode.h>
#include <graph/Graph.h>
#include <graph/NodeError.h>

#include "DSumFactory.h"
#include "DSumSampler.h"

#include <stdexcept>

using std::set;
using std::vector;
using std::runtime_error;

/*
static bool isDSumNode(Node const *node)
{
  StochasticNode const *snode = asStochastic(node);
  if (snode) {
    return snode->distribution()->name() == "dsum";
  }
  else {
    return false;
  }
}


static bool isDiscreteScalar(StochasticNode const *snode)
{
    return (node->length() == 1) && node->isDiscreteValued();
}

static bool 
canSample(vector<StochasticNode*> const &parameters, Graph const &graph)
{

    vector<StochasticNode*>::const_iterator p;
    for (p = parameters.begin(); p != parameters.end(); ++p) {

	if (!isDiscreteScalar(*p))
	    return false;
	
	//  Check that there is only a single child of param within the graph, 
	//  and that it is a dsum node
	int nchild = 0;
	for (set<Node*>::const_iterator i = param->children()->begin();
	     i != param->children()->end(); ++i)
	    {
		if (!isDSumNode(*i))
		    return false;
		
		++nchild;
	    }
	if (nchild != 1)
	    return false;
    }
    return true;
}
*/

void DSumFactory::makeSampler(set<StochasticNode*> &nodes,
			      Graph const &graph,
			      vector<vector<Sampler*> > &samplers) const
{
    set<StochasticNode*> dsum_nodes;
    set<StochasticNode*>::const_iterator p;

    //Find DSum nodes
    for (p = nodes.begin(); p != nodes.end(); ++p) {
	if ((*p)->distribution()->name() == "dsum")
	    dsum_nodes.insert(*p);
    }
  
    //See if we can sample them
    for (p = dsum_nodes.begin(); p != dsum_nodes.end(); ++p) {

	bool cansample = true;

	/* Get parents of dsumnode as a vector of Stochastic Nodes */
	vector<StochasticNode *> parameters;
	vector<Node const *> const &parents = (*p)->parents();
	vector<Node const *>::const_iterator pp;
	for (pp = parents.begin(); pp != parents.end(); ++pp) {
	    set<StochasticNode *>::const_iterator q =
		find(nodes.begin(), nodes.end(), *pp);
	    if (q == nodes.end()) {
		cansample = false;
		break;
	    }
	    else {
		parameters.push_back(*q);
	    }
	}

	if (cansample && DSumSampler::canSample(parameters, graph)) {
	    for (unsigned int i = 0; i < parameters.size(); ++i) {
		nodes.erase(parameters[i]);
	    }
	    for (unsigned int ch = 0; ch < samplers.size(); ++ch) {
		samplers[ch].push_back(new DSumSampler(parameters, graph, ch));
	    }
	}
    }
}
