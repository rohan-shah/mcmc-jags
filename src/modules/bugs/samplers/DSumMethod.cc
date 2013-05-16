#include <config.h>
#include <distribution/Distribution.h>
#include <graph/StochasticNode.h>
#include <graph/Graph.h>
#include <sampler/GraphView.h>

#include "DSumMethod.h"

#include <cfloat>
#include <climits>
#include <set>
#include <algorithm>
#include <cmath>

#include <JRmath.h>

using std::set;
using std::vector;
using std::max;
using std::min;
using std::exp;
using std::string;
using std::floor;

namespace jags {
namespace bugs {

DSumMethod::DSumMethod(GraphView const *gv, unsigned int chain)
    : Slicer(2, 10), _gv(gv), _chain(chain),
      _sum(gv->stochasticChildren()[0]->value(chain)[0]),
      _discrete(gv->stochasticChildren()[0]->isDiscreteValued())
{
    //Make sure values are consistent at start
    setValue(gv->nodes()[0]->value(chain)[0]);
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
    
	// Must be all discrete or none discrete
	if (nodes[i]->isDiscreteValued() != nodes[0]->isDiscreteValued())
	    return false;
    }

    /* Nodes must be direct parents of a single observed stochastic
       node with distribution DSum  */

    GraphView gv(nodes, graph);
    vector<DeterministicNode*> const &dchild = gv.deterministicChildren();
    vector<StochasticNode *> const &schild = gv.stochasticChildren();

    if (!dchild.empty())
	return false;
    if (schild.size() != 1)
	return false;
    if (!isObserved(schild[0]))
	return false;
    if (schild[0]->distribution()->name() != "dsum")
	return false;

    // And so, their work was done...
    return true;
}

void DSumMethod::setValue(double x)
{
    _x = x;
    double value[2];
    value[0] = _discrete ? floor(x) : x;
    value[1] = _sum - value[0];
    _gv->setValue(value, 2, _chain);
}

double DSumMethod::value() const
{
    return _x;
}

void DSumMethod::getLimits(double *lower, double *upper) const
{
    vector<StochasticNode *> const &n = _gv->nodes();
    double l0, u0, l1, u1;
    n[0]->support(&l0, &u0, 1U, _chain);
    n[1]->support(&l1, &u1, 1U, _chain);
    *lower = max(l0, _sum - u1);
    *upper = min(u0, _sum - l1);
}

void DSumMethod::update(RNG *rng)
{
    if (!updateDouble(rng)) {
	switch(state()) {
	case SLICER_POSINF:
	    throwNodeError(_gv->nodes().front(),
			   "Slicer stuck at value with infinite density");
	    break;
	case SLICER_NEGINF:
		throwNodeError(_gv->nodes().front(),
			       "Current value is inconsistent with data");
		break;
	case SLICER_OK:
	    break;
	}
	return false;
    }
    return true;
}

string DSumMethod::name() const
{
    return "DSumMethod";
}

double DSumMethod::logDensity() const
{
    return _gv->logFullConditional(_chain);
}

}}
