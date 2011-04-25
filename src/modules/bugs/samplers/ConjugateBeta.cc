#include <config.h>

#include "ConjugateBeta.h"
#include "ConjugateSampler.h"

#include <rng/RNG.h>
#include <graph/LogicalNode.h>
#include <graph/StochasticNode.h>
#include <graph/MixtureNode.h>
#include <sampler/GraphView.h>
#include <sampler/Linear.h>
#include <module/ModuleError.h>

#include <set>
#include <vector>
#include <cmath>
#include <algorithm>

#include <JRmath.h>

using std::vector;
using std::set;
using std::sqrt;
using std::max;
using std::min;
using std::string;

namespace bugs {

bool ConjugateBeta::canSample(StochasticNode *snode, Graph const &graph)
{
    switch(getDist(snode)) {
    case BETA:
	break;
    case UNIF:
	// dunif(0,1) is equivalent to dbeta(1,1) 
	if(!(*snode->parents()[0]->value(0) == 0 &&
	     *snode->parents()[1]->value(0) == 1 &&
	     snode->parents()[0]->isObserved() &&
	     snode->parents()[1]->isObserved()))
	    return false;
	break;
    default:
	return false;
    }

    GraphView gv(snode, graph);
    vector<DeterministicNode*> const &dchild = gv.deterministicChildren();
    vector<StochasticNode const*> const &schild = gv.stochasticChildren();

    // Check deterministic descendants
    // Only Mixture nodes are allowed
    for (unsigned int j = 0; j < dchild.size(); ++j) {
	if (!isMixture(dchild[j])) {
	    return false;
	}
    }
    if (!checkScale(&gv, false)) {
	return false;
    }

    // Check stochastic children
    for (unsigned int i = 0; i < schild.size(); ++i) {
	if (isBounded(schild[i])) {
	    return false; //Bounded
	}
	switch(getDist(schild[i])) {
	case BIN: case NEGBIN:
	    if (gv.isDependent(schild[i]->parents()[1])) {
		return false; //n depends on snode
	    }      
	    break;
	case BERN:
	    break;
	default:
	    return false;
	}
    }

    return true; //We made it!
}


ConjugateBeta::ConjugateBeta(GraphView const *gv)
    : ConjugateMethod(gv)
{
}

void ConjugateBeta::update(unsigned int chain, RNG *rng) const
{
    vector<StochasticNode const*> const &stoch_children = 
	_gv->stochasticChildren();
    StochasticNode const *snode = _gv->nodes()[0];

    double a, b;
    switch (_target_dist) {
    case BETA:
	a = *snode->parents()[0]->value(chain);
	b = *snode->parents()[1]->value(chain);
	break;
    case UNIF:
	a = 1;
	b = 1;
	break;
    default:
	throwLogicError("Invalid distribution in ConjugateBeta sampler");
    }
    unsigned int Nchild = stoch_children.size();

    /* For mixture models, we count only stochastic children that
       depend on snode */
    double *C = 0;
    bool is_mix = !_gv->deterministicChildren().empty();
    if (is_mix) {
	C = new double[Nchild];
	for (unsigned int i = 0; i < Nchild; ++i) {
	    C[i] = *stoch_children[i]->parents()[0]->value(chain);
	}
	// Perturb current value, keeping in the legal range [0,1]
	double x = *snode->value(chain);
	x = x > 0.5 ? x - 0.4 : x + 0.4;
	_gv->setValue(&x, 1, chain);
	// C[i] == 1 if parameter of child i has changed (so depends on snode)
	// C[i] == 0 otherwise
	for (unsigned int i = 0; i < Nchild; ++i) {
	    C[i] = (*stoch_children[i]->parents()[0]->value(chain) != C[i]);
	}
    }


    for (unsigned int i = 0; i < stoch_children.size(); ++i) {
	if (!(is_mix && C[i] == 0)) {
	    double y = *stoch_children[i]->value(chain);
	    double n, aplus, bplus;
	    switch(_child_dist[i]) {
	    case BIN:
		n = *stoch_children[i]->parents()[1]->value(chain);
		aplus = y;
		bplus = n - y;
		break;
	    case NEGBIN:
		n = *stoch_children[i]->parents()[1]->value(chain);
		aplus = n;
		bplus = y;
		break;
	    case BERN:
		aplus = y;
		bplus = 1 - y;
		break;
	    default:
		throwLogicError("Invalid distribution in Conjugate Beta sampler");
	    }
	    a += aplus;
	    b += bplus;
	}
    }

    // Draw the sample
    double xnew = rbeta(a, b, rng);
    if (isBounded(snode)) {
	double lower = 0;
	Node const *lb = snode->lowerBound();
	if (lb) {
	    lower = max(lower, *lb->value(chain));
	}
	double upper = 1;
	Node const *ub = snode->upperBound();
	if (ub) {
	    upper = min(upper, *ub->value(chain));
	}
	/* Try 4 more attempts to get random sample within the bounds */
	for (int i = 0; i < 4; i++) {
	    if (xnew >= lower && xnew <= upper) {
		_gv->setValue(&xnew, 1, chain);
                if (is_mix) delete [] C;
		return;
	    }
	    xnew = rbeta(a, b, rng);
	}
	/* Failure! Use inversion */
	double plower = lb ? pbeta(lower, a, b, 1, 0) : 0;
	double pupper = ub ? pbeta(upper, a, b, 1, 0) : 1;
	double p = runif(plower, pupper, rng);
	xnew = qbeta(p, a, b, 1, 0);   
    }
    _gv->setValue(&xnew, 1, chain);

    if (is_mix) {
	delete [] C;
    }
}

string ConjugateBeta::name() const
{
    return "ConjugateBeta";
}

}
