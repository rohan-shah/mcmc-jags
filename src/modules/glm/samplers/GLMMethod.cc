#include <config.h>
#include <rng/RNG.h>

#include <set>
#include <vector>
#include <stdexcept>
#include <algorithm>

#include "GLMMethod.h"

#include <sampler/Updater.h>
#include <sampler/Linear.h>
#include <graph/Graph.h>
#include <graph/StochasticNode.h>
#include <graph/DeterministicNode.h>
#include <graph/LinkNode.h>
#include <distribution/Distribution.h>
#include <rng/TruncatedNormal.h>

using std::string;
using std::vector;
using std::set;
using std::runtime_error;
using std::logic_error;
using std::copy;

static void getIndices(set<StochasticNode const *> const &schildren,
		       vector<StochasticNode const*> const &rows,
		       vector<int> &indices)
{
    indices.clear();
    
    for (unsigned int i = 0; i < rows.size(); ++i) {
	if (schildren.count(rows[i])) {
	    indices.push_back(i);
	}
    }
    
    if (indices.size() != schildren.size()) {
	throw logic_error("Size mismatch in getIndices");
    }
}

static Node const *getLinearPredictor(StochasticNode const *snode)
{
    Node const *lp = 0;

    switch(glm::GLMMethod::getFamily(snode)) {
    case GLM_NORMAL: case GLM_BERNOULLI: case GLM_BINOMIAL: case GLM_POISSON:
	lp = snode->parents()[0];
	break;
    case GLM_UNKNOWN:
	break;
    }
    
    LinkNode const *ln = dynamic_cast<LinkNode const*>(lp);
    if (ln) 
	lp = ln->parents()[0];
    
    return lp;
}

namespace glm {

    void GLMMethod::calDesign() const
    {
	vector<StochasticNode *> const &snodes = _updater->nodes();
	vector<StochasticNode const *> const &schildren = 
	    _updater->stochasticChildren();

	int *Xi = _X->i;
	int *Xp = _X->p;
	double *Xx = _X->x;
	
	int nrow = schildren.size();
	int ncol = _updater->length();
	if (nrow != _X->m || ncol != _X->n) {
	    throw logic_error("Dimension mismatch in GLMMethod::calDesign");
	}

	int c = 0; //column counter
	double *xnew = new double[_length_max];
	
	for (unsigned int i = 0; i < snodes.size(); ++i) {

	    unsigned int length = snodes[i]->length();

	    if (_init || !_fixed[i]) {

		for (unsigned int j = 0; j < length; ++j) {
		    for (int r = Xp[c+j]; r < Xp[c+j+1]; ++r) {
			Xx[r] = -getMean(Xi[r]);
		    }
		}
		
		double const *xold = snodes[i]->value(_chain);	    
		copy(xold, xold + length, xnew);
		
		for (unsigned int j = 0; j < length; ++j) {
		    
		    xnew[j] += 1;
		    _sub_updaters[i]->setValue(xnew, length, _chain);
		    for (int r = Xp[c+j]; r < Xp[c+j+1]; ++r) {
			Xx[r] += getMean(Xi[r]);
		    }
		    xnew[j] -= 1;
		}
		_sub_updaters[i]->setValue(xnew, length, _chain);
	    }
	    
	    c += length;
	}

	delete [] xnew;
	
    }
    
    GLMMethod::GLMMethod(Updater const *updater, 
			 vector<Updater const *> const &sub_updaters,
			 unsigned int chain, bool link)
	: _lp(updater->stochasticChildren().size()),
	  _updater(updater), _chain(chain), _sub_updaters(sub_updaters),
	  _X(0), _symbol(0), _fixed(sub_updaters.size(), false), 
	  _length_max(0), _nz_prior(0), _init(true)
    {
	vector<StochasticNode const*> const &schildren = 
	    updater->stochasticChildren();

	int nrow = schildren.size();
	int ncol = updater->length();

	//Set up linear predictor
	for (unsigned int i = 0; i < nrow; ++i) {
	    _lp[i] = getLinearPredictor(schildren[i])->value(chain);
	}

	vector<int> Xp(ncol + 1);
	vector<int> Xi;
    
	int c = 0; //column counter
	int r = 0; //count of number of non-zero entries

	for (unsigned int p = 0; p < _sub_updaters.size(); ++p) {

	    set<StochasticNode const *> children_p;
	    children_p.insert(sub_updaters[p]->stochasticChildren().begin(),
			      sub_updaters[p]->stochasticChildren().end());
	    vector<int> indices;
	    getIndices(children_p, schildren, indices);

	    unsigned int length = _sub_updaters[p]->length();
	    for (unsigned int i = 0; i < length; ++i, ++c) {
		Xp[c] = r;
		for (unsigned int j = 0; j < indices.size(); ++j, ++r) {
		    Xi.push_back(indices[j]);
		}
	    }

	    //Save these values for later calculations
	    _nz_prior += length * length; //No. of non-zeros in prior precision
	    if (length > _length_max) {
		_length_max = length; //Length of longest sampled node
	    }
	}
	Xp[c] = r;

	//Set up sparse representation of the design matrix
	_X = cs_spalloc(nrow, ncol, r, 1, 0);
	copy(Xp.begin(), Xp.end(), _X->p);
	copy(Xi.begin(), Xi.end(), _X->i);

	// Check for constant linear terms
	for (unsigned int i = 0; i < sub_updaters.size(); ++i) {
	    _fixed[i] = checkLinear(sub_updaters[i], true, link);
	}


    }

    GLMMethod::~GLMMethod()
    {
	cs_spfree(_X);
    }
    
    /* 
       Symbolic analysis of the posterior precision matrix for the
       Cholesky decomposition.
       
       This only needs to be done once, when the GLMMethod is
       craeted. It is a stripped-down version of the code in update.
       Note that the values of the sparse matrices are never
       referenced.
    */
    void GLMMethod::symbolic()  
    {
	unsigned int nrow = _updater->length();
	cs *Aprior = cs_spalloc(nrow, nrow, _nz_prior, 0, 0); 
    
	// Prior contribution
	int *Ap = Aprior->p;
	int *Ai = Aprior->i;

	int c = 0;
	int r = 0;
	vector<StochasticNode*> const &snodes = _updater->nodes();
	for (vector<StochasticNode*>::const_iterator p = snodes.begin();
	     p != snodes.end(); ++p)
	{
	    StochasticNode *snode = *p;
	    unsigned int length = snode->length();

	    int cbase = c; //first column in this diagonal block
	    for (unsigned int i = 0; i < length; ++i, ++c) {
		Ap[c] = r;
		for (unsigned int j = 0; j < length; ++j, ++r) {
		    Ai[r] = cbase + j;
		}
	    }
	}
	Ap[c] = r;

	// Likelihood contribution
    
	cs *t_X = cs_transpose(_X, 0);
	cs *Alik = cs_multiply(t_X, _X);
	cs *A = cs_add(Aprior, Alik, 0, 0);
    
	//Free working matrices
	cs_spfree(t_X);
	cs_spfree(Aprior);
	cs_spfree(Alik);

	_symbol = cs_schol(1, A); 
	cs_spfree(A);
    }

    void GLMMethod::calCoef(double *&b, cs *&A) 
    {
	//   The log of the full conditional density takes the form
	//   -(t(x) %*% A %*% x - 2 * b %*% x)/2
	//   where A is the posterior precision and the mean mu solves
	//   A %*% mu = b

	//   For computational convenience we take xold, the current value
	//   of the sampled nodes, as the origin

	unsigned int nrow = _updater->length();
	b = new double[nrow];
	cs *Aprior = cs_spalloc(nrow, nrow, _nz_prior, 1, 0); 
    
	// Set up prior contributions to A, b
	int *Ap = Aprior->p;
	int *Ai = Aprior->i;
	double *Ax = Aprior->x;

	int c = 0;
	int r = 0;
	vector<StochasticNode*> const &snodes = _updater->nodes();
	for (vector<StochasticNode*>::const_iterator p = snodes.begin();
	     p != snodes.end(); ++p)
	{
	    StochasticNode *snode = *p;
	    double const *priormean = snode->parents()[0]->value(_chain);
	    double const *priorprec = snode->parents()[1]->value(_chain);
	    double const *xold = snode->value(_chain);
	    unsigned int length = snode->length();
	
	    int cbase = c; //first column of this diagonal block
	    for (unsigned int i = 0; i < length; ++i, ++c) {
		b[c] = 0;
		Ap[c] = r;
		for (unsigned int j = 0; j < length; ++j, ++r) {
		    b[c] += priorprec[i + length*j] * (priormean[j] - xold[j]);
		    Ai[r] = cbase + j;
		    Ax[r] = priorprec[i + length*j];
		}
	    }
	}
	Ap[c] = r;

	// Recalculate the design matrix, if necessary
	calDesign();

	// Likelihood contributions
	//   
	//   b += t(X) %*% tau %*% (Y - mu)
	//   A += t(X) %*% tau %*% X
    
	//   where 
	//   - X is the design matrix
	//   - tau is the (diagonal) variance matrix of the stochastic children
	//   - mu is the mean of the stochastic children
	//   - Y is the value of the stochastic children

	cs *t_X = cs_transpose(_X, 1);
	int *Tp = t_X->p;
	int *Ti = t_X->i;
	double *Tx = t_X->x;
    
	for (int c = 0; c < t_X->n; ++c) {
	    double tau = getPrecision(c);
	    double delta = getValue(c) - getMean(c);
	    for (int r = Tp[c]; r < Tp[c+1]; ++r) {
		Tx[r] *= tau;
		b[Ti[r]] += Tx[r] * delta;
	    
	    }
	}

	cs *Alik = cs_multiply(t_X, _X);
	cs_spfree(t_X);
	A = cs_add(Aprior, Alik, 1, 1);
	cs_spfree(Aprior);
	cs_spfree(Alik);
    }

    void GLMMethod::updateLM(RNG *rng, bool stochastic) 
    {
	//   The log of the full conditional density takes the form
	//   -(t(x) %*% A %*% x - 2 * b %*% x)/2
	//   where A is the posterior precision and the mean mu solves
	//   A %*% mu = b

	//   For computational convenience we take xold, the current value
	//   of the sampled nodes, as the origin

	if (_init) {
	    initAuxiliary(rng);
	    calDesign();
	    symbolic();
	    _init = false;
	}

	double *b = 0;
	cs *A = 0;
	calCoef(b, A);

	// Get Cholesky decomposition of posterior precision
	csn *N = cs_chol(A, _symbol);
	cs_spfree(A);
	if (!N) {
	    delete [] b;
	    throw runtime_error("Cholesky decomposition failure in GLMMethod");
	}

	// Use the Cholesky decomposition to generate a new sample
	// with mean mu such that A %*% mu = b and precision A. The
	// vector b is overwritten with the result
	
	unsigned int nrow = _updater->length();
	double *w = new double[nrow];
	cs_ipvec(_symbol->pinv, b, w, nrow);
	cs_lsolve(N->L, w);
	updateAuxiliary(w, N, rng);
	if (stochastic) {
	    for (unsigned int r = 0; r < nrow; ++r) {
		w[r] += rng->normal();
	    }
	}
	cs_ltsolve(N->L, w);
	cs_nfree(N);
	cs_pvec(_symbol->pinv, w, b, nrow);
	delete [] w;
    
	//Shift origin back to original scale
	int r = 0;
	for (vector<StochasticNode*>::const_iterator p = 
		 _updater->nodes().begin();  p != _updater->nodes().end(); ++p)
	{
	    unsigned int length = (*p)->length();
	    double const *xold = (*p)->value(_chain);
	    for (unsigned int i = 0; i < length; ++i, ++r) {
		b[r] += xold[i];
	    }
	}

	_updater->setValue(b, nrow, _chain);
	delete [] b;
    }

    void GLMMethod::updateLMGibbs(RNG *rng) 
    {
	// Update element-wise. Less efficient than updateLM but
	// does not require a Cholesky decomposition, and is 
	// necessary for truncated parameters

	if (_init) {
	    if (_updater->length() != _sub_updaters.size()) {
		throw logic_error("updateLMGibbs can only act on scalar nodes");
	    }
	    initAuxiliary(rng);
	    calDesign();
	    _init = false;
	}

	double *b = 0;
	cs *A = 0;
	calCoef(b, A);

	int nrow = _updater->length();
	vector<double> theta(nrow);
	_updater->getValue(theta, _chain);

	//Extract diagonal from A
	vector<double> diagA(nrow);
	for (int c = 0; c < nrow; ++c) {
	    for (int j = A->p[c]; j < A->p[c+1]; ++j) {
		if (A->i[j] == c) {
		    diagA[c] = A->x[j];
		    break;
		}
	    }
	}

	//Update element-wise
	for (int i = 0; i < nrow; ++i) {
	    
	    double theta_old = theta[i];
		
	    double mu  = theta[i] + b[i]/diagA[i];
	    double sigma = sqrt(1/diagA[i]);
	    StochasticNode const *snode = _sub_updaters[i]->nodes()[0];
	    double const *l = snode->lowerLimit(_chain);
	    double const *u = snode->upperLimit(_chain);
		
	    if (l && u) {
		theta[i] = inormal(*l, *u, rng, mu, sigma);
	    }
	    else if (l) {
		theta[i] = lnormal(*l, rng, mu, sigma);
	    }
	    else if (u) {
		theta[i] = rnormal(*u, rng, mu, sigma);
	    }
	    else {
		theta[i] = mu + rng->normal() * sigma;
	    }
		
	    double delta = theta[i] - theta_old;
	    for (int j = A->p[i]; j < A->p[i+1]; ++j) {
		b[A->i[j]] -= delta * A->x[j];
	    }
	}


	_updater->setValue(theta,  _chain);

    }

    bool GLMMethod::isAdaptive() const
    {
	return false;
    }
    
    bool GLMMethod::adaptOff()
    {
	return true;
    }

    double GLMMethod::getMean(unsigned int i) const
    {
	return *_lp[i];
    }

    GLMFamily GLMMethod::getFamily(StochasticNode const *snode)
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
	else if (name == "dnorm") {
	    return GLM_NORMAL;
	}
	else {
	    return GLM_UNKNOWN;
	}
    }

    void GLMMethod::initAuxiliary(RNG *rng)
    {
    }
    
    void GLMMethod::updateAuxiliary(double *b, csn *N, RNG *rng)
    {
    }

}
