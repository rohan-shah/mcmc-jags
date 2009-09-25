#include <config.h>

#include "ConjugateGamma.h"

#include <graph/LogicalNode.h>
#include <graph/StochasticNode.h>
#include <graph/MixtureNode.h>
#include <graph/NodeError.h>
#include <sarray/SArray.h>
#include <sampler/Linear.h>
#include <sampler/Updater.h>

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

static double 
getScale(StochasticNode const *snode, ConjugateDist d, unsigned int chain)
{
    //Get scale parameter of snode
    switch(d) {
    case GAMMA: case NORM: case DEXP: case WEIB: case LNORM:
	return *snode->parents()[1]->value(chain);
	break;
    case EXP: case POIS:
	return *snode->parents()[0]->value(chain);
	break;
    default:
	throw NodeError(snode, 
			"Can't get scale parameter: invalid distribution");
    } 
}

static void calCoef(double *coef, Updater const *updater,
		    vector<ConjugateDist> const &child_dist, unsigned int chain)
{   
    const double xold = updater->nodes()[0]->value(chain)[0];
    vector<StochasticNode const*> const &stoch_children =
        updater->stochasticChildren();
    unsigned long nchildren = stoch_children.size();
    
    for (unsigned int i = 0; i < nchildren; ++i) {
        coef[i] = -getScale(stoch_children[i], child_dist[i], chain);
    }
    double val = xold + 1;
    updater->setValue(&val, 1, chain);
    for (unsigned int i = 0; i < nchildren; ++i) {
        coef[i] += getScale(stoch_children[i], child_dist[i], chain);
    }
    updater->setValue(&xold, 1, chain);
}

ConjugateGamma::ConjugateGamma(Updater const *updater)
    : ConjugateMethod(updater), _coef(0)
{
    if(!updater->deterministicChildren().empty() && checkScale(updater, true)) 
    {
	//One-off calculation of fixed scale transformation
	_coef = new double[updater->stochasticChildren().size()];
	calCoef(_coef, updater, _child_dist, 0);
    }

}

ConjugateGamma::~ConjugateGamma()
{
    delete [] _coef;
}


bool ConjugateGamma::canSample(StochasticNode *snode, Graph const &graph)
{
    Updater updater(vector<StochasticNode*>(1,snode), graph);
    
    switch (getDist(snode)) {
    case GAMMA: case EXP: case CHISQ:
	//The exponential and chisquare distributions are both special
	//cases of the gamma distribution and are handled by the conjugate
	//gamma method.
	break;
    default:
	return false;
    }

    // Create a set of nodes containing snode and its deterministic
    // descendants for the checks below.
    set<Node const *> paramset;
    paramset.insert(snode);
    vector<DeterministicNode*> const &dtrm_nodes = 
	updater.deterministicChildren();
    for (unsigned int j = 0; j < dtrm_nodes.size(); ++j) {
	paramset.insert(dtrm_nodes[j]);
    }

    // Check stochastic children
    vector<StochasticNode const*> const &stoch_nodes = 
	updater.stochasticChildren();
    for (unsigned int i = 0; i < stoch_nodes.size(); ++i) {
	vector<Node const*> const &param = stoch_nodes[i]->parents();
	if (isBounded(stoch_nodes[i])) {
	    return false; //Bounded
	}
	switch(getDist(stoch_nodes[i])) {
	case EXP: case POIS:
	    break;
	case GAMMA: case NORM: case DEXP: case WEIB: case LNORM:
	    if (paramset.count(param[0])) {
		return false; //non-scale parameter depends on snode
	    }
	    break;
	default:
	    return false;
	}
    }

    // Check deterministic descendants are scale transformations 
    if (!checkScale(&updater, false)) {
	return false;
    }
    return true; //We made it!
}


void 
ConjugateGamma::update(Updater *updater, unsigned int chain, RNG *rng) const
{
    vector<StochasticNode const*> const &stoch_children = 
	updater->stochasticChildren();
    unsigned int nchildren = stoch_children.size();

    double r; // shape
    double mu; // 1/scale

    //Prior
    vector<Node const *> const &param = updater->nodes()[0]->parents();
    switch(_target_dist) {
    case GAMMA:
	r = *param[0]->value(chain);
	mu = *param[1]->value(chain);
	break;
    case EXP:
	r = 1;
	mu = *param[0]->value(chain);
	break;
    case CHISQ:
	r = *param[0]->value(chain)/2;
	mu = 1/2;
	break;
    default:
	throw logic_error("invalid distribution in ConjugateGamma method");
    }

    // likelihood 
    double *coef = 0;
    bool empty = updater->deterministicChildren().empty();
    bool temp_coef = false;
    if (!empty && _coef == 0) {
	    temp_coef = true;
	    coef = new double[nchildren];
	    calCoef(coef, updater, _child_dist, chain);
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
	    switch(_child_dist[i]) {
	    case GAMMA:
		r += m;
		mu += coef_i * Y ;
		break;
	    case EXP:
		r += 1;
		mu += coef_i * Y;
		break;
	    case NORM:
		r += 0.5;
		mu += coef_i * (Y - m) * (Y - m) / 2;
		break;
	    case POIS:
		r += Y;
		mu += coef_i;
		break;
	    case DEXP:
		r += 1;
		mu += coef_i * fabs(Y - m);
		break;
	    case WEIB:
		r += 1; 
		mu += coef_i * pow(Y, m);
		break;
	    case LNORM:
		r+= 0.5;
		mu += coef_i * (log(Y) - m) * (log(Y) - m) / 2;
		break;
	    default:
		throw logic_error("Invalid distribution in Conjugate Gamma method");
	    }
	}
    }
    if (temp_coef) {
	delete [] coef;
    }

    // Sample from the posterior
    double xnew;
    if (isBounded(updater->nodes()[0])) {
	// Use inversion to get random sample
	double lower = 0;
	Node const *lb = updater->nodes()[0]->lowerBound();
	if (lb) {
	    lower = max(lower, *lb->value(chain));
	}
	Node const *ub = updater->nodes()[0]->upperBound();
	double plower = lb ? pgamma(lower,             r, 1/mu, 1, 0) : 0;
	double pupper = ub ? pgamma(*ub->value(chain), r, 1/mu, 1, 0) : 1;
	double p = runif(plower, pupper, rng);
	xnew = qgamma(p, r, 1/mu, 1, 0);    
    }
    else {
	xnew = rgamma(r, 1/mu, rng);
    }
    updater->setValue(&xnew, 1, chain);  
}

string ConjugateGamma::name() const
{
    return "ConjugateGamma";
}
