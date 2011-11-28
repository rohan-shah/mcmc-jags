#include <config.h>

#include "ShiftedPoisson.h"

//#include <graph/LogicalNode.h>
#include <graph/StochasticNode.h>
//#include <graph/MixtureNode.h>
//#include <graph/NodeError.h>
//#include <sarray/SArray.h>
//#include <sampler/Linear.h>
#include <sampler/GraphView.h>
//#include <module/ModuleError.h>

#include <JRmath.h>

using std::vector;
using std::string;

namespace bugs {

    ShiftedPoisson::ShiftedPoisson(GraphView const *gv)
	: ConjugateMethod(gv)
    {
    }

    bool ShiftedPoisson::canSample(StochasticNode *snode, Graph const &graph)
    {
	if (getDist(snode) != POIS)  
	    return false;
	if (isBounded(snode))
	    return false;

	GraphView gv(vector<StochasticNode*>(1,snode), graph);

	// There should be no deterministic children
	if (!gv.deterministicChildren().empty())
	    return false;

	// Check stochastic children: Must have single child node ...
	vector<StochasticNode const*> const &child_nodes = 
	    gv.stochasticChildren();
	if (child_nodes.size() != 1) 
	    return false;
	
	// ... with a Binomial(p, N) distribution such that N is the
	// sampled node and p does not depend on sampled node
	StochasticNode const *cnode = child_nodes.front();
	if (getDist(cnode) != BIN) 
	    return false;
	if (isBounded(cnode))
	    return false;
	if (cnode->parents()[1] != snode)
	    return false;
	if (cnode->parents()[0] == snode)
	    return false;

	return true;
    }


    void ShiftedPoisson::update(unsigned int chain, RNG *rng) const
    {
	StochasticNode const* snode = _gv->nodes()[0];
	StochasticNode const* cnode = _gv->stochasticChildren().front();

	double xmin = *cnode->value(chain); //Minimum value of shifted poisson
	double lambda = *snode->parents()[0]->value(chain); //Prior rate
	double pi = *cnode->parents()[0]->value(chain); //Binomial probability

	double xnew = xmin + rpois((1 - pi) * lambda, rng);
	_gv->setValue(&xnew, 1, chain);

    }

    string ShiftedPoisson::name() const
    {
	return "ShiftedPoisson";
    }

}
