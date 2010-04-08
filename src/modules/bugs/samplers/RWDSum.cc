#include <config.h>
#include <rng/RNG.h>
#include <sampler/GraphView.h>
#include <graph/Graph.h>
#include <graph/StochasticNode.h>
#include <distribution/Distribution.h>

#include "RWDSum.h"
#include <cmath>

using std::vector;
using std::log;
using std::exp;
using std::fabs;

//Target acceptance probability for Metropolis-Hastings algorithm
#define PROB  0.5

RWDSum::RWDSum(vector<double> const &value, double step, 
	       GraphView const *gv, unsigned int chain)
    : Metropolis(value), _step_adapter(step, PROB), _gv(gv), 
      _chain(chain), _pmean(0), _niter(2)
{
}

void RWDSum::rescale(double p)
{
    _step_adapter.rescale(p);

    // We keep a weighted mean estimate of the mean acceptance probability
    //  with the weights in favour of more recent iterations
    _pmean += 2 * (p - _pmean) / _niter;
    _niter++;
}

void RWDSum::update(RNG *rng)
{
    unsigned int n = length();
    vector<double> value(n);
    getValue(value);

    for (unsigned int i = 0; i < n - 1; ++i) {
	double log_p = -_gv->logFullConditional(_chain);
	step(value, _step_adapter.stepSize(), rng);
	setValue(value);
	log_p += _gv->logFullConditional(_chain);
	accept(rng, exp(log_p));
    }

}

bool RWDSum::checkAdaptation() const
{
    if (_pmean == 0 || _pmean == 1) {
	return false;
    }

    return fabs(_step_adapter.logitDeviation(_pmean)) < 0.5;
}

bool RWDSum::canSample(vector<StochasticNode *> const &nodes,
		       Graph const &graph, bool discrete)
{
    if (nodes.size() < 2)
	return false;

    for (unsigned int i = 0; i < nodes.size(); ++i) {
	if (!graph.contains(nodes[i]))
	    return false;

	// Nodes must be scalar ...
	if (nodes[i]->length() != 1)
	    return false;
    
	// Full rank
	if (nodes[i]->df() != 1)
	    return false;

	// Check discreteness
	if (nodes[i]->isDiscreteValued() != discrete)
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

    //Check that we have all the parent nodes
    if (schild[0]->parents().size() != nodes.size())
	return false;

    if (discrete) {
	//Check discreteness of child node
	if (!schild[0]->isDiscreteValued())
	    return false;
	//Check that value is really integer
	double val = schild[0]->value(0)[0];
	if (val != static_cast<int>(val)) {
	    return false;
	}
    }

    // And so, their work was done...
    return true;
}

void RWDSum::setValue(vector<double> const &value)
{
    _gv->setValue(value, _chain);
}

void RWDSum::getValue(vector<double> &value) const
{
    _gv->getValue(value, _chain);
}
