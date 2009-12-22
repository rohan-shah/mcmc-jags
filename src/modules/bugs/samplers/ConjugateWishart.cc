#include <config.h>

#include "ConjugateWishart.h"
#include "DWish.h"

#include <rng/RNG.h>
#include <graph/LogicalNode.h>
#include <graph/StochasticNode.h>
#include <graph/MixtureNode.h>
#include <sampler/Linear.h>
#include <sampler/Updater.h>

#include <set>
#include <stdexcept>
#include <vector>
#include <cmath>

#include <JRmath.h>

using std::vector;
using std::set;
using std::sqrt;
using std::invalid_argument;
using std::logic_error;
using std::string;

bool ConjugateWishart::canSample(StochasticNode *snode, Graph const &graph)
{
    if (getDist(snode) != WISH)
	return false;

    if (isBounded(snode))
	return false;
  
    Updater updater(snode, graph);
    vector<StochasticNode const*> const &schild = updater.stochasticChildren();
    vector<DeterministicNode*> const &dchild = updater.deterministicChildren();

    /* 
       Create a set of nodes containing snode and its deterministic
       descendants for the checks below.
    */
    set<Node const *> paramset;
    paramset.insert(snode);
    paramset.insert(dchild.begin(), dchild.end());

    // Check stochastic children
    for (unsigned int i = 0; i < schild.size(); ++i) {
	vector<Node const*> const &param = schild[i]->parents();
	if (isBounded(schild[i])) {
	    return false; //Bounded
	}
	switch(getDist(schild[i])) {
	case MNORM:
	    if (paramset.count(param[0])) {
		return false; //mean parameter depends on snode
	    }
	    break;
	default:
	    return false;
	}
    }
  
    if (!dchild.empty()) {
	// Deterministic children must be scale functions
	if (!checkScale(&updater, false)) {
	    return false;
	}
	// Only mixture nodes are allowed.  If we allowed arbitrary
	// functions, the complexity would be O(nrow^4).
	for (unsigned int i = 0; i < dchild.size(); ++i) {
	    if (!isMixture(dchild[i]))
		return false;
	}
    }

    return true;
}

ConjugateWishart::ConjugateWishart(Updater const *updater)
    : ConjugateMethod(updater)
{}

void 
ConjugateWishart::update(Updater *updater, unsigned int chain, RNG *rng) const
{
    vector<StochasticNode const*> const &stoch_children = 
	updater->stochasticChildren();
    unsigned int nchildren = stoch_children.size();

    vector<Node const *> const &param = updater->nodes()[0]->parents();  

    double df = *param[1]->value(chain);
    double const *Rprior = param[0]->value(chain);
    int nrow = param[0]->dim()[0];

    int N = nrow * nrow;
    double *R = new double[N]; 
    for (int i = 0; i < N; ++i) {
	R[i] = Rprior[i];
    }

    //Logical mask to determine which stochastic children are active.
    vector<bool> active(nchildren, true);

    if (!updater->deterministicChildren().empty()) {
	//Save first element of precision matrix for each child
	vector<double> precision0(nchildren); 
	for (unsigned int i = 0; i < nchildren; ++i) {
	    precision0[i] = stoch_children[i]->value(chain)[0];
	}
	//Double the current value
	double const *x = updater->nodes()[0]->value(chain);
	double *x2 = new double[N];
	for (int j = 0; j < N; ++j) {
	    x2[j] = 2 * x[j];
	}
	updater->setValue(x2, N, chain);
	delete [] x2;
	//See if precision matrix has changed
	for (unsigned int i = 0; i < nchildren; ++i) {
	    if (stoch_children[i]->value(chain)[0] == precision0[i]) {
		active[i] = false; //not active
	    }
	}
    }

    double *delta = new double[nrow];
    for (unsigned int i = 0; i < nchildren; ++i) {
	if (active[i]) {
	    StochasticNode const *schild = stoch_children[i];
	    double const *Y = schild->value(chain);
	    double const *mu = schild->parents()[0]->value(chain);
	    
	    for (int j = 0; j < nrow; j++) {
		delta[j] = Y[j] - mu[j];
	    }
	    for (int j = 0; j < nrow; j++) {
		for (int k = 0; k < nrow; k++) {
		    R[j*nrow + k] += delta[j] * delta[k];
		}
	    }
	    df += 1;
	}
    }
    delete [] delta;

    double *xnew = new double[N];
    DWish::randomSample(xnew, N, R, df, nrow, rng);

    delete [] R;
    updater->setValue(xnew, N, chain);
    delete [] xnew;
}

string ConjugateWishart::name() const
{
    return "ConjugateWishart";
}
