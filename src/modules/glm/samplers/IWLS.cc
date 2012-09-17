#include <config.h>
#include "IWLS.h"
#include <graph/LinkNode.h>
#include <graph/StochasticNode.h>
//#include <function/LinkFunction.h>
#include <distribution/Distribution.h>
#include <rng/RNG.h>
#include <module/ModuleError.h>

#include <cmath>

extern "C" {
#include <cholmod.h>
}

extern cholmod_common *glm_wk;

using std::string;
using std::vector;
using std::exp;
using std::fabs;
using std::log;
using std::copy;

static unsigned int nchildren(GraphView const *view)
{
    return view->stochasticChildren().size();
}

static double logDet(cholmod_factor *F)
{
    //NB Requiring simplicial factorization here. See glm.cc.

    double *Fx = static_cast<double*>(F->x);
    int *Fp = static_cast<int*>(F->p);

    double y = 0;    
    for (unsigned int r = 0; r < F->n; ++r) {
	y += log(Fx[Fp[r]]);
    }
    return F->is_ll ? 2*y : y;
}

#define MAX_ITER 100

namespace glm {


    IWLS::IWLS(GraphView const *view, 
	       vector<GraphView const *> const &sub_views,
	       unsigned int chain)
	: GLMMethod(view, sub_views, chain, true),
	  _link(nchildren(view)), _family(nchildren(view)), 
	  _init(true), _w(0)
    {
	vector<StochasticNode const*> const &children =
	    view->stochasticChildren();
	
	for (unsigned int i = 0; i < children.size(); ++i) {
	    _link[i] = dynamic_cast<LinkNode const*>(children[i]->parents()[0]);
	    _family[i] = getFamily(children[i]);
	    if ((_link[i] == 0) != (_family[i] == GLM_NORMAL)) {
		throwLogicError("Invalid link");
	    }
	}
    }
    
    string IWLS::name() const
    {
	return "IWLS";
    }

    double IWLS::getPrecision(unsigned int i) const
    {
	double w = _w;
	if(_family[i] == GLM_BINOMIAL || _family[i] == GLM_NORMAL) {
	    Node const *scale = _view->stochasticChildren()[i]->parents()[1];
	    w *= scale->value(_chain)[0];
	}

	double grad = _link[i] ? _link[i]->grad(_chain) : 1;
	return (w * grad * grad)/ var(i);
    }

    double IWLS::getValue(unsigned int i) const
    {
	Node const *child = _view->stochasticChildren()[i];

	double y = child->value(_chain)[0];
	if (_family[i] == GLM_BINOMIAL) {
	    double N = child->parents()[1]->value(_chain)[0];
	    y /= N;
	}

	if (_link[i] == 0) {
	    return y;
	}
	else {
	    double mu = _link[i]->value(_chain)[0];
	    double eta = _link[i]->eta(_chain);
	    double grad = _link[i]->grad(_chain);
	    
	    return eta + (y - mu) / grad;
	}
    }
 
    double IWLS::var(unsigned int i) const
    {
	double mu = _link[i] ? _link[i]->value(_chain)[0] : 0;

	switch(_family[i]) {
	case GLM_BERNOULLI: case GLM_BINOMIAL:
	    return  mu * (1 - mu);
	    break;
	case GLM_POISSON:
	    return mu;
	    break;
	case GLM_NORMAL:
	    return 1;
	    break;
	case GLM_UNKNOWN:
	    throwLogicError("Unknown GLM family in IWLS");
	}

	return 0; //-Wall
    }

    double IWLS::logPTransition(vector<double> const &xold, 
				vector<double> const &xnew,
				double *b, cholmod_sparse *A)
    {
	A->stype = -1;
	int ok = cholmod_factorize(A, _factor, glm_wk);
	if (!ok) {
	    throwRuntimeError("Cholesky decomposition failure in IWLS");
	}
	unsigned int n = _factor->n;
	
	//Difference between new and old values
	vector<double> dx(n);
	for (unsigned int i = 0; i < n; ++i) {
	    dx[i] = xnew[i] - xold[i];
	}

	//Make permuted copy of b
	cholmod_dense *w = cholmod_allocate_dense(n, 1, n, CHOLMOD_REAL, 
						  glm_wk);
	int *perm = static_cast<int*>(_factor->Perm);
	double *wx = static_cast<double*>(w->x);
	for (unsigned int i = 0; i < n; ++i) {
	    wx[i] = b[perm[i]];
	}

	//Posterior mean
	cholmod_dense *mu = cholmod_solve(CHOLMOD_LDLt, _factor, w, glm_wk);
	double *mux = static_cast<double*>(mu->x);

	//Setup pointers to sparse matrix A
	int *Ap = static_cast<int*>(A->p);
	int *Ai = static_cast<int*>(A->i);
	double *Ax = static_cast<double*>(A->x);

	double deviance = 0;
	for (unsigned int r = 0; r < n; ++r) {
	    double Adr = 0;
	    for (int j = Ap[r]; j < Ap[r+1]; ++j) {
		Adr += Ax[j] * dx[Ai[j]];
	    }
	    deviance += dx[r] * (Adr - 2 * b[r])  + wx[r] * mux[r];
	}
	deviance -= logDet(_factor);

	cholmod_free_dense(&w, glm_wk);
	cholmod_free_dense(&mu, glm_wk);

	return -deviance/2;
    }
		
    void IWLS::update(RNG *rng)
    {
	if (_init) {
	    _w = 0;
	    for (unsigned int i = 0; i < MAX_ITER; ++i) {
		_w += 1.0/MAX_ITER;
		updateLM(rng, false);
            }
	    _init = false;
	    return;
	}
	
	double *b1, *b2;
	cholmod_sparse *A1, *A2;
	double logp = 0;

	vector<double> xold(_view->length());
	_view->getValue(xold, _chain);
	calCoef(b1, A1);

	logp -= _view->logFullConditional(_chain);
	updateLM(rng);
	logp += _view->logFullConditional(_chain);

	vector<double> xnew(_view->length());
	_view->getValue(xnew, _chain);
	calCoef(b2, A2);

	logp -= logPTransition(xold, xnew, b1, A1);
	logp += logPTransition(xnew, xold, b2, A2);

	cholmod_free_sparse(&A1, glm_wk);
	cholmod_free_sparse(&A2, glm_wk);
	delete [] b1; delete [] b2;
	
	if (logp < 0 && rng->uniform() > exp(logp)) {
	    _view->setValue(xold, _chain); //reject proposal
	}
    }
}
