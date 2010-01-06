#include <config.h>

#include "ConjugateDirichlet.h"

#include <rng/RNG.h>
#include <graph/MixtureNode.h>
#include <graph/StochasticNode.h>
#include <graph/NodeError.h>
#include <sampler/GraphView.h>
#include <sampler/Linear.h>

#include <set>
#include <stdexcept>
#include <vector>
#include <cmath>

#include <JRmath.h>

using std::vector;
using std::set;
using std::sqrt;
using std::logic_error;
using std::string;

bool ConjugateDirichlet::canSample(StochasticNode *snode, Graph const &graph)
{
    if(getDist(snode) != DIRCH)
	return false;

    if (isBounded(snode))
	return false;

    GraphView gv(snode, graph);
    vector<DeterministicNode*> const &dchild = gv.deterministicChildren();
    vector<StochasticNode const*> const &schild = gv.stochasticChildren();

    // Check stochastic children
	for (unsigned int i = 0; i < schild.size(); ++i) {
	vector<Node const *> const &param = schild[i]->parents();
	if (isBounded(schild[i])) {
	    return false; //Truncated
	}
	switch(getDist(schild[i])) {
	case CAT:
	    break;
	case MULTI:
	    if (param[1] == snode)
		return false;
	    break;
	default:
	    return false;
	}
    }

    // Check deterministic descendants
    for (unsigned int j = 0; j < dchild.size(); ++j) {
	if (!isMixture(dchild[j])) {
	    return false;
	}
    }
    if (!checkScale(&gv, false)) {
	return false;
    }
  
    return true;
}

static bool allzero(double const *x, long length)
{
    for (long i = 0; i < length; ++i) {
	if (x[i])
	    return false;
    }
    return true;
}

ConjugateDirichlet::ConjugateDirichlet(GraphView const *gv)
    : ConjugateMethod(gv)
{}

void ConjugateDirichlet::update(unsigned int chain, RNG *rng) const
{
    StochasticNode *snode = _gv->nodes()[0];
    unsigned long size = snode->length();
    double *alpha = new double[size];
    double const *prior = snode->parents()[0]->value(chain);
    for (unsigned long i = 0; i < size; ++i) {
	alpha[i] = prior[i];
    }

    /* Set value to zero:
       This is an illegal value. It is used to find relevant
       children in mixture models.
       FIXME: We can ignore this in non-mixture models
    */
    double *xnew = new double[size];
    for (unsigned long i = 0; i < size; ++i) {
	xnew[i] = 0;
    }
    _gv->setValue(xnew, size, chain);

    vector<StochasticNode const*> const &stoch_children = 
	_gv->stochasticChildren();
    unsigned int nchildren = stoch_children.size();
    for (unsigned int i = 0; i < nchildren; ++i) {
	StochasticNode const *schild = stoch_children[i];
	long index = 0;
	double const *N = 0;
	if (allzero(schild->parents()[0]->value(chain), 
		    schild->parents()[0]->length())) {
	    switch(_child_dist[i]) {
	    case MULTI:
		N = schild->value(chain);
		for (unsigned long j = 0; j < size; ++j) {
		    alpha[j] += N[j];
		}
		break;
	    case CAT:
		index = static_cast<long>(*schild->value(chain) + 1.0E-6);
		alpha[index - 1] += 1;
		break;
	    default:
		throw logic_error("Invalid distribution in Conjugate Dirichlet method");
	    }
	}
    }

    /* Check structural zeros */
    for (unsigned long i = 0; i < size; ++i) {
	if (prior[i] == 0 && alpha[i] != 0) {
	    throw NodeError(snode, "Invalid likelihood for Dirichlet distribution with structural zeros");
	}
    }
  
    /* 
       Draw Dirichlet sample by drawing independent gamma random
       variates and then normalizing
    */

    double xsum = 0.0;
    for (unsigned long i = 0; i < size; ++i) {
	if (alpha[i] > 0) {
	    xnew[i] = rgamma(alpha[i], 1, rng);
	    xsum += xnew[i];
	}
	else {
	    xnew[i] = 0;
	}
    }
    for (unsigned long i = 0; i < size; ++i) {
	xnew[i] /= xsum;
    }

    _gv->setValue(xnew, size, chain);

    delete [] xnew;
    delete [] alpha;
}

string ConjugateDirichlet::name() const
{
    return "ConjugateDirichlet";
}
