#include <config.h>
#include <distribution/Distribution.h>
#include <graph/StochasticNode.h>
#include <graph/Graph.h>
#include <sampler/ParallelDensitySampler.h>
#include "DSumSampler.h"

#include <cfloat>
#include <climits>
#include <stdexcept>
#include <set>
#include <algorithm>
#include <cmath>

#include <JRmath.h>

using std::set;
using std::vector;
using std::invalid_argument;
using std::max;
using std::min;
using std::exp;

DSumMethod::DSumMethod()
    : Slicer(2, 10)
{
}

void DSumMethod::initialize(ParallelDensitySampler *sampler, unsigned int chain)
{
    _sampler = sampler;
    _chain = chain;

    vector<StochasticNode *> const &nodes = sampler->nodes();
    vector<StochasticNode const *> const &stoch_children = 
	sampler->stochasticChildren();

    /* One of the stochastic children of the sampled nodes is an observed
       StochasticNode with a DSum distribution. Find it */
    Node const *dsum = 0;
    for (unsigned int i = 0; i < stoch_children.size(); ++i) {
	StochasticNode const *child = stoch_children[i];
	if (child->isObserved() && child->distribution()->name() == "dsum") {
	    dsum = child;
	    break;
	}
    }
    
    _sum = static_cast<long>(*dsum->value(chain));
    _x = *nodes[0]->value(chain);

    //Make sure values are consistent at start
    double x2 = _sum - static_cast<long>(_x);
    nodes[1]->setValue(&x2,1,chain);
}

DSumMethod::~DSumMethod()
{
}

bool DSumMethod::canSample(vector<StochasticNode *> const &nodes,
			    Graph const &graph)
{
    if (nodes.size() != 2)
	return false;

    if (!(graph.contains(nodes[0]) && graph.contains(nodes[1])))
	return false;
    
    for (unsigned int i = 0; i < 2; ++i) {
	// Nodes must be scalar ...
	if (nodes[i]->length() != 1)
	    return false;
    
	// discrete-valued ...
	if (!nodes[i]->isDiscreteValued())
	    return false;
    }

    /* Nodes must be direct parents of a single observed stochastic
       node with distribution DSum  */
    vector<StochasticNode const*> stoch_nodes;
    vector<Node*> dtrm_nodes;
    Sampler::classifyChildren(nodes, graph, stoch_nodes, dtrm_nodes);
    if (!dtrm_nodes.empty())
	return false;
    if (stoch_nodes.size() != 1)
	return false;
    if (!stoch_nodes[0]->isObserved())
	return false;
    if (stoch_nodes[0]->distribution()->name() != "dsum")
	return false;

    // And so, their work was done...
    return true;
}

void DSumMethod::setValue(double x)
{
    _x = x;
    double value[2];
    value[0] = static_cast<long>(x);
    value[1] = _sum - value[0];
    _sampler->setValue(value, 2, _chain);
}

double DSumMethod::value() const
{
    return _x;
}

void DSumMethod::getLimits(double *lower, double *upper) const
{
    vector<StochasticNode *> const &n = _sampler->nodes();
    double l0, u0, l1, u1;
    support(&l0, &u0, 1U, n[0], _chain);
    support(&l1, &u1, 1U, n[1], _chain);
    /*
    n[0]->distribution()->support(&l0, &u0, 1U, n[0]->parameters(_chain),
				  n[0]->parameterDims());
    n[1]->distribution()->support(&l1, &u1, 1U, n[1]->parameters(_chain),
				  n[1]->parameterDims());
    */
    *lower = max(l0, _sum - u1);
    *upper = min(u0, _sum - l1);
}

void DSumMethod::update(RNG *rng)
{
    updateDouble(rng);
}
