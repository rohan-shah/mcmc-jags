#include <config.h>

#include "ConjugateDirichlet.h"

#include <rng/RNG.h>
#include <graph/MixtureNode.h>
#include <graph/StochasticNode.h>
#include <graph/AggNode.h>
#include <graph/NodeError.h>
#include <sampler/GraphView.h>
#include <sampler/Linear.h>
#include <util/logical.h>

#include <set>
#include <stdexcept>
#include <vector>
#include <cmath>
#include <algorithm>

#include <JRmath.h>

using std::vector;
using std::set;
using std::sqrt;
using std::logic_error;
using std::string;
using std::copy;

static bool checkAggNode(AggNode const *anode, StochasticNode const *snode)
{
    /* 
       Check that the sample node (snode) is embedded entirely within
       the aggregate node (anode). We do this by checking that all
       elements of snode are included once (and only once) in anode.
    */
    unsigned int n = snode->length();
    vector<Node const *> const &par = anode->parents();
    vector<unsigned int> const &off = anode->offsets();
    vector<bool> offmask(n, false);
    for (unsigned int i = 0; i < par.size(); ++i) {
	if (par[i] == snode) {
	    unsigned int j = off[i];
	    if (j > n) {
		return false; //Invalid offset
	    }
	    if (offmask[j]) {
		return false; //Duplicate offset
	    }
	    else {
		offmask[j] = true;
	    }
	}
    }
    return allTrue(offmask);
}

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
	    if (gv.isDependent(param[1]))
		return false;
	    break;
	default:
	    return false;
	}
    }

    // Check deterministic descendants
    for (unsigned int j = 0; j < dchild.size(); ++j) {
	AggNode const *achild = dynamic_cast<AggNode const*>(dchild[j]);
	if (achild) {
	    if (!checkAggNode(achild, snode)) return false;
	}
	else if (!isMixture(dchild[j])) {
	    return false;
	}
    }
    if (!checkLinear(&gv, false)) {
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

static bool findMix(GraphView const *gv)
{
    //Find mixture nodes among deterministic descendants
    vector<DeterministicNode*> const &dchild = gv->deterministicChildren();
    for (unsigned int i = 0; i < dchild.size(); ++i) {
	if (isMixture(dchild[i])) return true;
    }
    return false;
}

static bool findAgg(GraphView const *gv)
{
    //Find aggregate nodes among deterministic descendants
    vector<DeterministicNode*> const &dchild = gv->deterministicChildren();
    for (unsigned int i = 0; i < dchild.size(); ++i) {
	if (dynamic_cast<AggNode const *>(dchild[i])) return true;
    }
    return false;
}

static void calOffsets(GraphView const *gv, unsigned int chain, int *offsets)
{
    vector<StochasticNode const *> const &schild = gv->stochasticChildren();
    unsigned int nchildren = schild.size();
    StochasticNode const *snode = gv->nodes()[0];
    unsigned int length = snode->length();

    //Store current value of sampled node
    double const *x = snode->value(chain);
    vector<double> xold(length);
    copy(x, x + length, xold.begin());

    //Store current values of parameters of stochastic children
    vector<double const *> params(nchildren);
    vector<unsigned int> npar(nchildren);

    unsigned int N = 0;
    for (unsigned int i = 0; i < nchildren; ++i) {
	params[i] = schild[i]->parents()[0]->value(chain);
	npar[i] = schild[i]->parents()[0]->length();
	N += npar[i];
    }
    double *par = new double[N];
    double *pp = par;
    for (unsigned int i = 0; i < nchildren; ++i) {
	pp = copy(params[i], params[i] + npar[i], pp);
    }
    
    //Perturb value of snode to find indices
    for (unsigned int j = 0; j < length; ++j) {

	vector<double> xnew = xold;
	xnew[j] += 1;
	gv->setValue(xnew, chain);

	pp = par;
	int *op = offsets;

	for (unsigned int i = 0; i < nchildren; ++i) {
	    op[j] = -1;
	    for (unsigned int k = 0; k < npar[i]; ++k) {
		if (params[i][k] != pp[k]) {
		    op[j] = k;
		    break;
		}
	    }
	    pp += npar[i];
	    op += length;
	}
    }
    //Reset to old value
    gv->setValue(xold, chain);

    delete [] par;

    //double check negative indices
    int *op = offsets;
    for (unsigned int i = 0; i < nchildren; ++i) {
	for (unsigned int j = 1; j < length; ++j) {
	    if ((op[0] == -1) != (op[j] == -1))
		throw logic_error("Bad offset in ConjugateDirichlet");
	}
	op += length;
    }
}

ConjugateDirichlet::ConjugateDirichlet(GraphView const *gv)
    : ConjugateMethod(gv), _mix(findMix(gv)), _agg(findAgg(gv)), _offsets(0)
{
    if (_agg && !_mix) {
	//One-time calculation of offsets
	unsigned int nchildren = gv->stochasticChildren().size();
	unsigned int length = gv->length();
	_offsets = new int[nchildren * length];
	calOffsets(gv, 0, _offsets);
    }
}

ConjugateDirichlet::~ConjugateDirichlet()
{
    delete [] _offsets;
}

void ConjugateDirichlet::update(unsigned int chain, RNG *rng) const
{
    StochasticNode *snode = _gv->nodes()[0];
    unsigned long size = snode->length();
    double *alpha = new double[size];
    double *xnew = new double[size];

    double const *prior = snode->parents()[0]->value(chain);
    for (unsigned long i = 0; i < size; ++i) {
	alpha[i] = prior[i];
    }

    vector<StochasticNode const*> const &schild = _gv->stochasticChildren();
    unsigned int nchildren = schild.size();
    
    /* 
       Calculation of the likelihood contributions may be more or less
       complicated depending on whether the deterministic descendants
       include aggregate nodes and/or mixture nodes.
    */

    int *offsets = 0;
    if (_mix) {
	//Mixture models
	if (_agg) {
	    //Need to re-calculate offsets at each iteration
	    offsets = new int[size * nchildren];
	    calOffsets(_gv, chain, offsets);
	}
	else {
	    //We can find active nodes just by setting the current
	    //value of the sampled node to zero. 
	    for (unsigned int i = 0; i < size; ++i) {
		xnew[i] = 0;
	    }
	    _gv->setValue(xnew, size, chain);
	}
    }
    else {
	//Non mixture models. 
	if (_agg) {
	    //Offsets do not change. Use pre-calculated offsets
	    offsets = _offsets;
	}
    }

    int *op = offsets;
    for (unsigned int i = 0; i < nchildren; ++i) {
	int index = 0;
	double const *N = 0;
	if (offsets) {
	    if (op[0] != -1) {
		switch(_child_dist[i]) {
		case MULTI:
		    N = schild[i]->value(chain);
		    for (unsigned int j = 0; j < size; ++j) {
			alpha[j] += N[op[j]];
		    }
		    break;
		case CAT:
		    index = static_cast<int>(*schild[i]->value(chain)) - 1;
		    for (unsigned int j = 0; j < size; ++j) {
			if (index == op[j])
			    alpha[j] += 1;
		    }
		    break;
		default:
		    throw logic_error("Invalid distribution in Conjugate Dirichlet method");
		}
	    }
	    op += size;
	}
	else {
	    bool active = true;
	    if (_mix) {
		active = allzero(schild[i]->parents()[0]->value(chain),
				 schild[i]->parents()[0]->length());
	    }
	    if (active) {
		switch(_child_dist[i]) {
		case MULTI:
		    N = schild[i]->value(chain);
		    for (unsigned int j = 0; j < size; ++j) {
			alpha[j] += N[j];
		    }
		    break;
		case CAT:
		    index = static_cast<int>(*schild[i]->value(chain)) - 1;
		    alpha[index] += 1;
		    break;
		default:
		    throw logic_error("Invalid distribution in Conjugate Dirichlet method");
		}
	    }
	}
    }

    if (offsets && _offsets == 0) {
	delete [] offsets;
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
