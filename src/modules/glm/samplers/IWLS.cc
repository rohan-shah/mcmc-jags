#include <config.h>
#include "IWLS.h"
#include <graph/LinkNode.h>
#include <graph/StochasticNode.h>
#include <function/InverseLinkFunc.h>
#include <distribution/Distribution.h>
#include <rng/RNG.h>

#include <stdexcept>
#include <cmath>

using std::string;
using std::vector;
using std::logic_error;
using std::exp;
using std::fabs;

static unsigned int nchildren(Updater const *updater)
{
    return updater->stochasticChildren().size();
}

#define MAX_ITER 50
#define TOL 1.0E-3

#include <iostream>

namespace glm {


    IWLS::IWLS(Updater const *updater, 
	       vector<Updater const *> const &sub_updaters,
	       unsigned int chain)
	: GLMMethod(updater, sub_updaters, chain, true),
	  _link(nchildren(updater)), _family(nchildren(updater)), 
	  _init(true)
    {
	vector<StochasticNode const*> const &children =
	    updater->stochasticChildren();
	
	for (unsigned int i = 0; i < children.size(); ++i) {
	    _link[i] = dynamic_cast<LinkNode const*>(children[i]->parents()[0]);
	    _family[i] = getFamily(children[i]);
	}
    }
    
    string IWLS::name() const
    {
	return "IWLS";
    }

    double IWLS::getMean(unsigned int i) const
    {
	return _link[i]->eta(_chain);
    }

    double IWLS::getPrecision(unsigned int i) const
    {
	double w = 1;
	if (_family[i] == GLM_BINOMIAL) {
	    Node const *size = _updater->stochasticChildren()[i]->parents()[1];
	    w *= size->value(_chain)[0];
	}

	double grad = _link[i]->grad(_chain);
	return (w * grad * grad)/ var(i);
    }

    double IWLS::getValue(unsigned int i) const
    {
	Node const *child = _updater->stochasticChildren()[i];

	double y = child->value(_chain)[0];
	if (_family[i] == GLM_BINOMIAL) {
	    double N = child->parents()[1]->value(_chain)[0];
	    y /= N;
	}

	double mu = _link[i]->value(_chain)[0];
	double eta = _link[i]->eta(_chain);
	double grad = _link[i]->grad(_chain);

	return eta + (y - mu) / grad;
    }
 
    double IWLS::var(unsigned int i) const
    {
	double mu = _link[i]->value(_chain)[0];

	switch(_family[i]) {
	case GLM_BERNOULLI: case GLM_BINOMIAL:
	    return  mu * (1 - mu);
	    break;
	case GLM_POISSON:
	    return mu;
	    break;
	case GLM_UNKNOWN:
	    throw logic_error("Unknown GLM family in IWLS");
	}
	return 0; //-Wall
    }
 
    GLMFamily IWLS::getFamily(StochasticNode const *snode)
    {
	string const &name = snode->distribution()->name();
	if (name == "dbern") {
	    return GLM_BERNOULLI;
	}
	else if (name == "dbin") {
	    return GLM_BINOMIAL;
	}
	else if (name == "dpois") {
	    return GLM_POISSON;
	}
	else {
	    return GLM_UNKNOWN;
	}
    }

    void IWLS::initAuxiliary(RNG *rng)
    {
	//No auxiliary variables in this method
    }
    
    void IWLS::updateAuxiliary(double *b, csn const *N, RNG *rng)
    {
	//No auxiliary variables in this method
    }

    double IWLS::logPTransition(vector<double> const &xorig, 
			       vector<double> const &x,
			       double const *b, cs const *A)
    {
	unsigned int n = _updater->length();
	
	csn *N = cs_chol(A, _symbol);
	if (!N) {
	    throw logic_error("Cholesky decomposition failure in IWLS");
	}

	double *w = new double[n];
	double *mu = new double[n];

	cs const *L = N->L;
	cs_ipvec(_symbol->pinv, b, w, n);
	cs_lsolve(L, w);
	cs_ltsolve(L, w);
	cs_pvec(_symbol->pinv, w, mu, n);

	for (unsigned int i = 0; i < n; ++i) {
	    w[i] = x[i] - xorig[i] - mu[i];
	}
	cs_ipvec(_symbol->pinv, w, mu, n);
	delete [] w;

	double logp = 0;
	int const *Li = L->i;
	int const *Lp = L->p;
	double const *Lx = L->x;
	for (unsigned int i = 0; i < n; ++i) {
	    double y = 0;
	    for (int j = Lp[i]; j < Lp[i+1]; ++j) {
		y += mu[Li[j]] * Lx[j];
	    }
	    logp += log(Lx[Lp[i]]) - y * y/2;
	}
	delete [] mu;
	cs_nfree(N);

	return logp;
    }
		
    void IWLS::update(RNG *rng)
    {
	if (_init) {
	    /* 
	       Set initial value of parameters to maximum likelihood
	       estimate using standard IWLS algorithm
	    */
	    double logden = _updater->logFullConditional(_chain);
	    for (unsigned int i = 0; i < MAX_ITER; ++i) {
		updateLM(rng, false);
		double logden1 = _updater->logFullConditional(_chain);
		if (fabs(logden1 - logden) < TOL) {
		    break;
		}
		else {
		    logden = logden1;
		}
	    }
	    _init = false;
	}

	
	double *b1, *b2;
	cs *A1, *A2;
	double logp = 0;
	
	vector<double> xold(_updater->length());
	_updater->getValue(xold, _chain);
	calCoef(b1, A1);
	
	logp -= _updater->logFullConditional(_chain);
	updateLM(rng);
	logp += _updater->logFullConditional(_chain);
	
	vector<double> xnew(_updater->length());
	_updater->getValue(xnew, _chain);
	calCoef(b2, A2);
	
	logp -= logPTransition(xold, xnew, b1, A1);
	logp += logPTransition(xnew, xold, b2, A2);
	
	cs_spfree(A1); cs_spfree(A2);
	delete [] b1; delete [] b2;
	
	//Acceptance step
	if (rng->uniform() > exp(logp)) {
	    _updater->setValue(xold, _chain);
	}
    }
}
