#include <config.h>
#include <distribution/Distribution.h>
#include <graph/StochasticNode.h>
#include <graph/Graph.h>
#include <sampler/Updater.h>
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
using std::string;

DSumMethod::DSumMethod(Updater const *updater, unsigned int chain)
    : Slicer(2, 10), _updater(updater), _chain(chain),
      _x(updater->nodes()[0]->value(chain)[0]),
      _sum(static_cast<long>(updater->stochasticChildren()[0]->value(chain)[0]))
{
    //Make sure values are consistent at start
    double x2 = _sum - static_cast<long>(_x);
    updater->nodes()[1]->setValue(&x2, 1, chain);
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
    vector<DeterministicNode*> dtrm_nodes;
    Updater::classifyChildren(nodes, graph, stoch_nodes, dtrm_nodes);
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
    _updater->setValue(value, 2, _chain);
}

double DSumMethod::value() const
{
    return _x;
}

void DSumMethod::getLimits(double *lower, double *upper) const
{
    vector<StochasticNode *> const &n = _updater->nodes();
    double l0, u0, l1, u1;
    support(&l0, &u0, 1U, n[0], _chain);
    support(&l1, &u1, 1U, n[1], _chain);
    *lower = max(l0, _sum - u1);
    *upper = min(u0, _sum - l1);
}

void DSumMethod::update(RNG *rng)
{
    updateDouble(rng);
}

string DSumMethod::name() const
{
    return "DSumMethod";
}

double DSumMethod::logDensity() const
{
    return _updater->logFullConditional(_chain);
}
