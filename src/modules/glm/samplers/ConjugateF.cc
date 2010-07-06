#include <config.h>

#include "ConjugateGamma.h"

#include <graph/LogicalNode.h>
#include <graph/StochasticNode.h>
#include <graph/MixtureNode.h>
#include <graph/NodeError.h>
#include <sarray/SArray.h>
#include <sampler/Linear.h>
#include <sampler/GraphView.h>

#include <set>
#include <stdexcept>
#include <vector>
#include <cmath>
#include <algorithm>

#include <JRmath.h>

using std::vector;
using std::set;
using std::sqrt;
using std::invalid_argument;
using std::logic_error;
using std::max;
using std::sort;
using std::string;

static double getScale(StochasticNode const *snode, unsigned int chain)
{
    //Get precision parameter of normal distribution
    return *snode->parents()[1]->value(chain);
}

static void calCoef(double *coef, GraphView const *gv, unsigned int chain)
{   
    const double xold = gv->nodes()[0]->value(chain)[0];
    vector<StochasticNode const*> const &schildren = gv->stochasticChildren();
    unsigned long nchildren = schildren.size();
    
    for (unsigned int i = 0; i < nchildren; ++i) {
        coef[i] = -getScale(snode);
    }
    double val = xold + 1;
    gv->setValue(&val, 1, chain);
    for (unsigned int i = 0; i < nchildren; ++i) {
        coef[i] += getScale(snode);
    }
    gv->setValue(&xold, 1, chain);
}

static void getStochasticChildren(Node const *node, set<StochasticNode*> &sset)
{
    set<StochasticNode*> const &schild = node->stochasticChildren();
    sset.insert(schild.start(), schild.end());

    set<DeterministicChildren*> const &dchild = node->deterministicChildren();
    set<DeterministicChildren*>::const_iterator p;
    for (p = dchild.begin(); p != dchild.end(); ++p) {
	getStochasticChildren(*p);
    }
}

static void convertStochasticChildren(StochasticNode const *snode,
				      vector<StochasticNode const*> const &in,
				      vector<StochasticNode *> &out)
{
    set<StochasticNode*> sset;
    getStochasticChildren(snode, sset);

    out.clear();
    for (unsigned int i = 0; i < in.size(); ++i) {
	Node const *cnode = in[i];
	set<StochasticNode*>::const_iterator p = sset.begin();
	while (p != sset.end())
	{
	    if (*p == in[i]) {
		out.push_back(in[i]);
		break;
	    }
	    else {
		++p;
	    }
	}
	if (p == sset.end()) {
	    throw logic_error("Conversion failure in ConjugateF method");
	}
    }
}


bool ConjugateF::canSample(StochasticNode *snode, Graph const &graph)
{
    if (snode->distribution()->name() != "df") 
	return false;
    if (!snode->parents()[1]->isObserved())
	return false;
    if (*snode->parents()[1]->value(0) != 1)
	return false;
    if (snode->isBounded())
	return false; //FIXME. We could include this case

    GraphView gv(vector<StochasticNode*>(1,snode), graph);

    // Check stochastic children
    vector<StochasticNode const*> const &snodes = gv.stochasticChildren();
    for (unsigned int i = 0; i < snodes.size(); ++i) {
	if (snodes[i]->distribution()->name() != "dnorm") {
	    return false;
	}
	if (gv.isDependent(snodes[i]->parents()[0])) {
	    return false; //mean parameter depends on snode
	}
	if (snodes[i]->isBounded()) {
	    return false;
	}
	if (snodes[i]->isObserved()) {
	    return false;
	}
    }

    // Check deterministic descendants are scale transformations 
    if (!checkScale(&gv, false)) {
	return false;
    }

    // Need to get non-const pointers to stochastic children
    vector<StochasticNode*> schildren;
    convertStochasticChildren(snode, snodes, schildren);
    
    // Now check stochastic children to see if they form a linear
    // model

    GraphView gv2(schildren, graph);
    if (!checkLinear(&gv2, false, false))
	return false;

    vector<StochasticNode const *> const &snodes2 = gv2.stochasticChildren();
    for (unsigned int i = 0; i < snodes2.size(); ++i) {
	if (snodes2[i]->distribution()->name() != "dnorm") {
	    return false;
	}
	if (snodes2[i]->isBounded()) {
	    return false;
	}
	if (gv.isDependent(snodes2[i]->parents()[1])) {
	    return false; //Precision parameter is dependent
	}
    }

    return true;
}

static vector<StochasticNode*> getStochasticChildren(GraphView const &gv)
{
    StochasticNode *snode = gv->nodes()[0];
    vector<StochasticNode*> ans;
    convertStochasticChildren(snode, gv.stochasticChildren(), ans);
    return ans;
}

ConjugateF::ConjugateF(StochasticNode *snode, Graph const &graph, 
		       unsigned int chain)
    : _gv1(GraphView(vector<StochasticNode*>(1,snode), graph)),
      _gv2(GraphView(getStochasticChildren(_gv1), graph)),
      _chain(chain),
      _scale0(1), _tau0(node->value(chain)[0]),
      _coef(0)
{
    if(!_gv1->deterministicChildren().empty() && checkScale(gv, true)) 
    {
	//One-off calculation of fixed scale transformation
	_coef = new double[_gv1->stochasticChildren().size()];
	calCoef(_coef, _gv1, chain);
    }
}

ConjugateF::~ConjugateF()
{
    delete [] _coef;
}


void ConjugateF::update(RNG *rng) const
{
    unsigned int N = _gv2.length();

    /* Re-parameterize */
    vector<double> phi(N);
    _gv2.getValue(phi, _chain);

    vector<StochasticNode*> const &schildren = _gv2->nodes();
    for (unsigned int i = 0; i < N; ++i) {
	phi[i] -= schildren[i]->parents()[0]->value(chain)[0];
	phi[i] /= _scale0;
    }

    /* Update tau, which has a conjugate gamma distribution */

    //Prior
    vector<Node const *> const &param = _gv->nodes()[0]->parents();
    double r = 0.5; //shape
    double mu = *param[0]->value(chain)/2;
    
    // likelihood 
    double *coef = 0;
    bool empty = _gv->deterministicChildren().empty();
    bool temp_coef = false;
    if (!empty && _coef == 0) {
	    temp_coef = true;
	    coef = new double[nchildren];
	    calCoef(coef, _gv, chain);
    }
    else {
	coef = _coef;
    }

    for (unsigned int i = 0; i < nchildren; ++i) {

	double coef_i = empty ? 1 : coef[i];
	if (coef_i > 0) {

	    StochasticNode const *schild = stoch_children[i];
	    vector<Node const*> const &cparam = schild->parents();
	    double Y = *schild->value(chain);
	    double m = *cparam[0]->value(chain); //location parameter 
	    r += 0.5;
	    mu += coef_i;

	}
    }
    if (temp_coef) {
	delete [] coef;
    }

    // Sample from the posterior
    double xnew;
    if (isBounded(_gv->nodes()[0])) {
	// Use inversion to get random sample
	double lower = 0;
	Node const *lb = _gv->nodes()[0]->lowerBound();
	if (lb) {
	    lower = max(lower, *lb->value(chain));
	}
	Node const *ub = _gv->nodes()[0]->upperBound();
	double plower = lb ? pgamma(lower,             r, 1/mu, 1, 0) : 0;
	double pupper = ub ? pgamma(*ub->value(chain), r, 1/mu, 1, 0) : 1;
	double p = runif(plower, pupper, rng);
	xnew = qgamma(p, r, 1/mu, 1, 0);    
    }
    else {
	xnew = rgamma(r, 1/mu, rng);
    }
    _gv->setValue(&xnew, 1, chain);  
}

string ConjugateF::name() const
{
    return "ConjugateF";
}
