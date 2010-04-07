#include <config.h>
#include <distribution/Distribution.h>
#include <graph/StochasticNode.h>
#include <graph/Graph.h>
#include <sampler/GraphView.h>
#include <rng/RNG.h>

#include "DiscreteDSum.h"

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
using std::floor;

static vector<double> nodeValues(GraphView const *gv, unsigned int chain)
{
    unsigned int n = gv->nodes().size();
    vector<double> ans(n);
    for (unsigned int i = 0; i < n; ++i) {
	ans[i] = gv->nodes()[i]->value(chain)[0];
    }
    return ans;
}

DiscreteDSum::DiscreteDSum(GraphView const *gv, unsigned int chain,
			       unsigned int nrep)
    : RWMetropolis(nodeValues(gv, chain), 0.1), 
      _gv(gv), _chain(chain), _nrep(nrep), _ivalue(gv->nodes().size())
{
    //Ensure that values of sampled nodes are consistent with dsum parent

    unsigned int n = gv->nodes().size();
    double delta = gv->stochasticChildren()[0]->value(chain)[0];
    for (unsigned int i = 0; i < n; ++i) {
	delta -= gv->nodes()[i]->value(chain)[0];
    }
    int idelta = static_cast<int>(trunc(delta/n));
    for (unsigned int i =0; i < n; ++i) {
	double val = gv->nodes()[i]->value(chain)[0] + idelta;
	gv->nodes()[i]->setValue(&val, 1, chain);
    }
    double eps = delta - n * idelta;
    int ieps = static_cast<int>(eps);
    if (ieps != 0) {
	double val = gv->nodes()[0]->value(chain)[0] + ieps;
	gv->nodes()[0]->setValue(&val, 1, chain);
    }

    for (unsigned int i = 0; i < n; ++i) {
	_ivalue[i] = static_cast<int>(gv->nodes()[i]->value(chain)[0]);
    }

}

bool DiscreteDSum::canSample(vector<StochasticNode *> const &nodes,
			       Graph const &graph)
{
    if (nodes.size() < 2)
	return false;

    if (!(graph.contains(nodes[0]) && graph.contains(nodes[1])))
	return false;
    
    for (unsigned int i = 0; i < nodes.size(); ++i) {
	// Nodes must be scalar ...
	if (nodes[i]->length() != 1)
	    return false;

	// Full rank
	if (nodes[i]->df() != 1)
	    return false;

	// Must be discrete
	if (!nodes[i]->isDiscreteValued())
	    return false;
    }

    /* Nodes must be direct parents of a single observed stochastic
       node with distribution DSum  */

    GraphView gv(nodes, graph);
    vector<DeterministicNode*> const &dchild = gv.deterministicChildren();
    vector<StochasticNode const*> const &schild = gv.stochasticChildren();

    if (!dchild.empty())
	return false;
    if (schild.size() != 1)
	return false;
    if (!schild[0]->isObserved())
	return false;
    if (schild[0]->distribution()->name() != "dsum")
	return false;
    if (schild[0]->parents().size() != nodes.size())
	return false;

    // And so, their work was done...
    return true;
}

void DiscreteDSum::step(vector<double> &value, double s, RNG *rng) const
{
    for (unsigned int r = 0; r < _nrep; ++r) {

	//Randomly draw two components of the vector
	int n = value.size();
	int i = static_cast<int>(rng->uniform() * n);
	if (i == n) --i; 
	int j = static_cast<int>(rng->uniform() * (n - 1));
	if (j == n - 1) --j;
	if (j >= i) ++j;
	
	//Modify the chosen components while keeping the sum constant
	double eps = rng->normal() * s;
	int inteps = static_cast<int>(fabs(eps)) + 1;
	if (eps < 0) {
	    inteps = -inteps;
	}
	value[i] += inteps;
	value[j] -= inteps;

    }
}

string DiscreteDSum::name() const
{
    return "DiscreteDSum";
}

double DiscreteDSum::logDensity() const
{
    return _gv->logFullConditional(_chain);
}

void DiscreteDSum::setValue(vector<double> const &value)
{
    vector<double> dvalue(value.size());
    for (unsigned int i = 0; i < value.size(); ++i) {
	_ivalue[i] = static_cast<int>(value[i]);
	dvalue[i] = _ivalue[i];
    }

    _gv->setValue(dvalue, _chain);
}

void DiscreteDSum::getValue(vector<double> &value) const
{
    _gv->getValue(value, _chain);
}
