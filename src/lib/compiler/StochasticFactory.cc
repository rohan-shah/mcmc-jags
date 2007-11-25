#include <config.h>
#include <compiler/StochasticFactory.h>
#include <graph/StochasticNode.h>
#include <graph/NodeError.h>
#include <distribution/Distribution.h>
#include <compiler/NodeFactory.h>

#include <stdexcept>

using std::pair;
using std::map;
using std::vector;
using std::invalid_argument;

bool lt(StochasticTrio const &arg1, StochasticTrio const &arg2)
{
    if (arg1.dist < arg2.dist) {
	// Order first by distribution
	return true;
    }
    else if (arg1.dist > arg2.dist) {
	return false;
    }
    else if (lt(arg1.param, arg2.param)) {
	// Then by parameters
	return true;
    }
    else if (lt(arg2.param, arg1.param)) {
	return false;
    }
    else {
	// Then by data

	if (arg1.data.dim(true) < arg2.data.dim(true)) {
	    return true;
	}
	else if (arg2.data.dim(true) < arg1.data.dim(true)) {
	    return false;
	}
	else {
	    //return lt(arg1.data.value(), arg2.data.value(), arg1.data.length());
	    return lt(arg1.data.value(), arg2.data.value());
	}
    }
}

StochasticTrio::StochasticTrio(Distribution const *distarg, 
			     vector<Node const *> const &paramarg,
			     SArray const &dataarg)
 : dist(distarg), param(paramarg), data(dataarg)
{}

/* FIXME: we are currently restricted to unbounded nodes */
StochasticNode*
StochasticFactory::getStochasticNode(Distribution const *dist, 
				     vector<Node const *> const &parents,
				     SArray const &data,
				     Graph &graph)
{
    if (dist == 0) {
	throw invalid_argument("NULL distribution in getStochasticNode");
    }

    StochasticTrio strio(dist, parents, data);
    map<StochasticTrio, StochasticNode*, ltstochastic>::iterator 
	i = _stochasticmap.find(strio);

    if (i != _stochasticmap.end()) {
        i->second->replicate();
	return i->second;
    }
    else {
	// Create a new stochastic node
	StochasticNode *snode = new StochasticNode(dist, parents);
	graph.add(snode);

	if (snode->dim() != data.dim(true)) {
	    throw NodeError(snode, "Dimension mismatch between node and data");
	}
	snode->setObserved(data.value());
	_stochasticmap[strio] = snode;
	return snode;
    }
}
