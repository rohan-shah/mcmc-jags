#include <config.h>
#include <rng/RNG.h>

#include <set>
#include <vector>
#include <algorithm>
#include <cmath>

#include "GLMMethod.h"
#include "Outcome.h"

#include <sampler/SingletonGraphView.h>
#include <sampler/Linear.h>
#include <graph/Graph.h>
#include <graph/StochasticNode.h>
#include <graph/DeterministicNode.h>
#include <graph/LinkNode.h>
#include <distribution/Distribution.h>
#include <rng/TruncatedNormal.h>
#include <module/ModuleError.h>

using std::string;
using std::vector;
using std::set;
using std::copy;
using std::sqrt;

//debuggin
#include <cfloat>
using std::abs;

extern cholmod_common *glm_wk;

namespace jags {

static void getIndices(set<StochasticNode *> const &schildren,
		       vector<StochasticNode *> const &rows,
		       vector<int> &indices)
{
    indices.clear();
    
    for (unsigned int i = 0; i < rows.size(); ++i) {
	if (schildren.count(rows[i])) {
	    indices.push_back(i);
	}
    }
    
    if (indices.size() != schildren.size()) {
	throwLogicError("Size mismatch in getIndices");
    }
}

namespace glm {

    static double
    sparse_inprod(int const *ai, int const *ai_end, double const *ax, 
		  int const *bi, int const *bi_end, double const *bx,
		  vector<double> const &tau)
    {
	/*
	  Inner product of two sparse vectors: t(a) %*% diag(tau) %*% b

	  ai,bi          = start of non-zero indices of a,b. 
	  ai_end, bi_end = end of non-zero indices of a,b. 
	  ax, bx         = start of non-zero values of a,b
	  tau            = dense vector of scale factors

	  Assumptions: 
	  ax is the same length as ai
	  ai, bi are both increasing
	  ai, bi contain no values greater than tau.size() - 1
	*/
	
	double c = 0;
	while (ai < ai_end && bi < bi_end) {
	    if (*ai == *bi) {
		c += (*ax) * (*bx) * tau[*ai];
		++ai; ++ax;
		++bi; ++bx;
	    }
	    else if (*ai < *bi) {
		++ai; ++ax;
	    }
	    else {
		// *bi < *ai
		++bi; ++bx;
	    }
	}
	return c;
    }

    static void sparse_xtx (cholmod_sparse const *X, vector<double> const &tau,
			    cholmod_sparse *A)
    {
	/* 
	   Calculates t(X) %*% diag(tau) %*% X and adds the result
	   to A, a pre-existing sparse matrix.

	   Assumptions: If A[r,c] is a structural zero then X[,r] and
	   X[,c] have no rows in common.
	*/
	   
	if (X->sorted == 0)
	    throwLogicError("Input matrix X must be sorted in sparse_xtx");
	if (X->nrow != tau.size() || A->ncol != X->ncol || A->ncol != A->nrow)
	    throwLogicError("Input dimension mismatch in sparse_xtx");

	//Set up access to X
	int const *Xp = static_cast<int const*>(X->p);
	int const *Xi = static_cast<int const*>(X->i);
	double const *Xx = static_cast<double const*>(X->x);	

	//Set up access to A
	int const *Ap = static_cast<int const*>(A->p);
	int const *Ai = static_cast<int const*>(A->i);
	double *Ax = static_cast<double*>(A->x);
	
	// Iterate over the non-zero elements of A and add the inner
	// product of the corresponding row and column of X. 
	for (unsigned int col = 0; col < A->ncol; ++col) {
	    int const *ai = Xi + Xp[col];
	    int const *ai_end = Xi + Xp[col+1];
	    double const *ax = Xx + Xp[col];
	    for (int j = Ap[col]; j < Ap[col+1]; ++j) {
		int row = Ai[j];
		int const *bi = Xi + Xp[row];
		int const *bi_end = Xi + Xp[row+1];
		double const *bx = Xx + Xp[row];
		Ax[j] += sparse_inprod(ai, ai_end, ax, bi, bi_end, bx, tau);
	    }
	}
    }
    
    void GLMMethod::calDesign() const
    {
	vector<StochasticNode *> const &snodes = _view->nodes();
	vector<StochasticNode *> const &schildren = 
	    _view->stochasticChildren();

	int *Xi = static_cast<int*>(_x->i);
	int *Xp = static_cast<int*>(_x->p);
	double *Xx = static_cast<double*>(_x->x);
	
	unsigned int nrow = schildren.size();
	unsigned int ncol = _view->length();
	if (nrow != _x->nrow || ncol != _x->ncol) {
	    throwLogicError("Dimension mismatch in GLMMethod::calDesign");
	}

	int c = 0; //column counter
	double *xnew = new double[_length_max];
	
	for (unsigned int i = 0; i < snodes.size(); ++i) {

	    unsigned int length = snodes[i]->length();

	    if (_init || !_fixed[i]) {

		for (unsigned int j = 0; j < length; ++j) {
		    for (int r = Xp[c+j]; r < Xp[c+j+1]; ++r) {
			Xx[r] = -_outcomes[Xi[r]]->mean();
		    }
		}
		
		double const *xold = snodes[i]->value(_chain);	    
		copy(xold, xold + length, xnew);
		
		for (unsigned int j = 0; j < length; ++j) {
		    
		    xnew[j] += 1;
		    _sub_views[i]->setValue(xnew, length, _chain);
		    for (int r = Xp[c+j]; r < Xp[c+j+1]; ++r) {
			Xx[r] += _outcomes[Xi[r]]->mean();
		    }
		    xnew[j] -= 1;
		}
		_sub_views[i]->setValue(xnew, length, _chain);
	    }
	    
	    c += length;
	}

	delete [] xnew;
	
    }
    
    GLMMethod::GLMMethod(GraphView const *view, 
			 vector<SingletonGraphView const *> const &sub_views,
			 vector<Outcome *> const &outcomes,
			 unsigned int chain, bool link)
	: _view(view), _chain(chain), _sub_views(sub_views),
	  _outcomes(outcomes),
	  _x(0), _factor(0), _fixed(sub_views.size(), false), 
	  _length_max(0), _nz_prior(0), _init(true)
    {
	view->checkFinite(chain); //Check validity of initial values
	
	vector<StochasticNode *> const &schildren = 
	    view->stochasticChildren();

	int nrow = schildren.size();
	int ncol = view->length();

	vector<int> Xp(ncol + 1);
	vector<int> Xi;
    
	int c = 0; //column counter
	int r = 0; //count of number of non-zero entries

	for (unsigned int p = 0; p < _sub_views.size(); ++p) {

	    set<StochasticNode *> children_p;
	    children_p.insert(sub_views[p]->stochasticChildren().begin(),
			      sub_views[p]->stochasticChildren().end());
	    vector<int> indices;
	    getIndices(children_p, schildren, indices);

	    unsigned int length = _sub_views[p]->length();
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

	// Set up sparse representation of the design matrix
	// Compact, sorted, non-symmetric, containing r non-zero values
	_x = cholmod_allocate_sparse(nrow, ncol, r, 1, 1, 0, CHOLMOD_REAL,
				     glm_wk);
	int *_xp = static_cast<int*>(_x->p);
	int *_xi = static_cast<int*>(_x->i);

	copy(Xp.begin(), Xp.end(), _xp);
	copy(Xi.begin(), Xi.end(), _xi);

	// Check for constant linear terms
	for (unsigned int i = 0; i < sub_views.size(); ++i) {
	    _fixed[i] = checkLinear(sub_views[i], true, link);
	}
    }

    GLMMethod::~GLMMethod()
    {
	while(!_outcomes.empty()) {
	    delete _outcomes.back();
	    _outcomes.pop_back();
	}
	cholmod_free_sparse(&_x, glm_wk);
	cholmod_free_sparse(&_A, glm_wk);
    }
    
    /* 
       Symbolic analysis of the posterior precision matrix for the
       Cholesky decomposition.
       
       This only needs to be done once, when the GLMMethod is
       created. We use CHOLMOD functions to determine the non-zero
       pattern of A and its cholesky factor. Both of these are stored
       for later use.
    */
    void GLMMethod::symbolic()  
    {
	unsigned int nrow = _view->length();

	// Prior contribution 
	cholmod_sparse *Aprior =  
	    cholmod_allocate_sparse(nrow, nrow, _nz_prior, 1, 1, 0,
				    CHOLMOD_PATTERN, glm_wk); 
	int *Ap = static_cast<int*>(Aprior->p);
	int *Ai = static_cast<int*>(Aprior->i);

	int c = 0;
	int r = 0;
	vector<StochasticNode*> const &snodes = _view->nodes();
	for (vector<StochasticNode*>::const_iterator p = snodes.begin();
	     p != snodes.end(); ++p)
	{
	    StochasticNode *snode = *p;
	    unsigned int length = snode->length();

	    int cbase = c; //first column in this diagonal block
	    for (unsigned int j = 0; j < length; ++j, ++c) {
		Ap[c] = r;
		for (unsigned int i = 0; i < length; ++i, ++r) {
		    Ai[r] = cbase + i;
		}
	    }
	}
	Ap[c] = r;
	
	// Likelihood contribution
	// t(X) %*% tau %*% X
	// where tau is precision matrix of outcomes. We allow only
	// scalar outcomes so tau is diagonal and pattern matrix is the
	// same as t(X) %*% X.
	cholmod_sparse *t_x = cholmod_transpose(_x, 0, glm_wk);
	cholmod_sparse *Alik = cholmod_aat(t_x, 0, 0, 0, glm_wk);

	// Posterior precision matrix is the sum of prior and likelihood
	// contributions
	cholmod_sparse *A = cholmod_add(Aprior, Alik, 0, 0, 0, 0, glm_wk);

	A->stype = -1; // Symmetric matrix with values in lower triangle
	cholmod_sort(A, glm_wk); // Sorts columns, discarding upper triangle
	_A = cholmod_copy_sparse(A, glm_wk); // Store for later use in calCoef
	_factor = cholmod_analyze(A, glm_wk); // Store for later use in updateLM
	
	//Free working matrices
	cholmod_free_sparse(&t_x, glm_wk);
	cholmod_free_sparse(&Aprior, glm_wk);
	cholmod_free_sparse(&Alik, glm_wk);
	cholmod_free_sparse(&A, glm_wk);
    }


    void GLMMethod::calCoef(double *&b, cholmod_sparse *&A) 
    {
	//   The log of the full conditional density takes the form
	//   -(t(x) %*% A %*% x - 2 * b %*% x)/2
	//   where A is the posterior precision and the mean mu solves
	//   A %*% mu = b

	//   For computational convenience we take xold, the current value
	//   of the sampled nodes, as the origin

	b = new double[_view->length()];

	//Copy precision matrix from stored pattern matrix
	A = cholmod_copy_sparse(_A, glm_wk);
	cholmod_sparse_xtype(CHOLMOD_REAL, A, glm_wk);

	// Set access to A
	int const *Ap = static_cast<int const*>(A->p);
	int const *Ai = static_cast<int const*>(A->i);
	double *Ax = static_cast<double*>(A->x);

	//Initialize A
	for (unsigned int i = 0; i < A->nzmax; ++i) {
	    Ax[i] = 0;
	}

	// Prior contributions

	//   b = sigma %*% muprior
	//   A = sigma
    
	//   where:
	//   - sigma is the block-diagonal precision matrix of the sampled
	//           nodes
	//   - muprior is the prior mean of the stochastic children relative
	//             to the current value

	int c = 0; //column counter
	vector<StochasticNode*> const &snodes = _view->nodes();
	for (vector<StochasticNode*>::const_iterator p = snodes.begin();
	     p != snodes.end(); ++p)
	{
	    StochasticNode *snode = *p;
	    double const *priormean = snode->parents()[0]->value(_chain);
	    double const *priorprec = snode->parents()[1]->value(_chain);
	    double const *xold = snode->value(_chain);
	    unsigned int length = snode->length();

	    for (unsigned int i = 0; i < length; ++i, ++c) {

		// Contributions to b
		
		b[c] = 0;
		for (unsigned int j = 0; j < length; ++j) {
		    b[c] += priorprec[i + length*j] * (priormean[j] - xold[j]);
		}

		// Contributions to A.
		
		// We rely on the fact that A is sorted with only the
		// lower triangular elements stored. This allows us to
		// efficiently add the block-diagonal contribution from
		// the prior. The code includes a sanity check for this
		// assumption.
		
		double *a_c = Ax + Ap[c];    // Start of column c
		int const *r_c = Ai + Ap[c]; // row check

		int kmax = length - i;
		for (int k = 0; k < kmax; ++k) {
		    if (r_c[k] != c + k) {
			throwLogicError("Sorting error in calCoef");
		    }
		    a_c[k] = priorprec[i + length * (i + k)];
		}

	    }
	}

	// Recalculate the design matrix, if necessary
	calDesign();

	// Likelihood contributions
	//   
	//   b += t(X) %*% tau %*% (Y - mu)
	//   A += t(X) %*% tau %*% X
    
	//   where 
	//   - X is the design matrix
	//   - tau is the diagonal precision matrix of the stochastic children
	//   - mu is the mean of the stochastic children
	//   - Y is the value of the stochastic children

	//Set up access to design matrix
	int const *Xp = static_cast<int const*>(_x->p);
	int const *Xi = static_cast<int const*>(_x->i);
	double *Xx = static_cast<double*>(_x->x);

	//Add likelihood contribution to b
	vector<double> delta(_x->nrow, 0);
	vector<double> tau(_x->nrow);
	for (unsigned int r = 0; r < _x->nrow; ++r) {
	    tau[r] = _outcomes[r]->precision();
	    delta[r] += tau[r] * (_outcomes[r]->value() - _outcomes[r]->mean());
	}
	for (unsigned int c = 0; c < _x->ncol; ++c) {
	    for (int r = Xp[c]; r < Xp[c+1]; ++r) {
		b[c] += Xx[r] * delta[Xi[r]];
	    }
	}
	
	//Calculate t(X) %*% diag(tau) %*% X, adding result to A
	sparse_xtx(_x, tau, A);
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
	    calDesign();
	    symbolic();
	    _init = false;
	}

	double *b = 0;
	cholmod_sparse *A = 0;
	calCoef(b, A);

	// Get LDL' decomposition of posterior precision
	int ok = cholmod_factorize(A, _factor, glm_wk);
	cholmod_free_sparse(&A, glm_wk);
	if (!ok) {
	    throwRuntimeError("Cholesky decomposition failure in GLMMethod");
	}

	// Use the LDL' decomposition to generate a new sample
	// with mean mu such that A %*% mu = b and precision A. 
	
	unsigned int nrow = _view->length();
	cholmod_dense *w = cholmod_allocate_dense(nrow, 1, nrow, CHOLMOD_REAL, 
						  glm_wk);

	// Permute RHS
	double *wx = static_cast<double*>(w->x);
	int *perm = static_cast<int*>(_factor->Perm);
	for (unsigned int i = 0; i < nrow; ++i) {
	    wx[i] = b[perm[i]];
	}

	cholmod_dense *u1 = cholmod_solve(CHOLMOD_L, _factor, w, glm_wk);
	updateAuxiliary(u1, _factor, rng);

	if (stochastic) {
	    double *u1x = static_cast<double*>(u1->x);
	    if (_factor->is_ll) {
		// LL' decomposition
		for (unsigned int r = 0; r < nrow; ++r) {
		    u1x[r] += rng->normal();
		}
	    }
	    else {
		// LDL' decomposition. The diagonal D matrix is stored
		// as the diagonal of _factor
		int *fp = static_cast<int*>(_factor->p);
		double *fx = static_cast<double*>(_factor->x);
		for (unsigned int r = 0; r < nrow; ++r) {
		    u1x[r] += rng->normal() * sqrt(fx[fp[r]]);
		}
	    }
	}

	cholmod_dense *u2 = cholmod_solve(CHOLMOD_DLt, _factor, u1, glm_wk);

	// Permute solution
	double *u2x = static_cast<double*>(u2->x);
	for (unsigned int i = 0; i < nrow; ++i) {
	    b[perm[i]] = u2x[i];
	}

	cholmod_free_dense(&w, glm_wk);
	cholmod_free_dense(&u1, glm_wk);
	cholmod_free_dense(&u2, glm_wk);

	//Shift origin back to original scale
	int r = 0;
	for (vector<StochasticNode*>::const_iterator p = 
		 _view->nodes().begin();  p != _view->nodes().end(); ++p)
	{
	    unsigned int length = (*p)->length();
	    double const *xold = (*p)->value(_chain);
	    for (unsigned int i = 0; i < length; ++i, ++r) {
		b[r] += xold[i];
	    }
	}

	_view->setValue(b, nrow, _chain);
	delete [] b;
    }

    void GLMMethod::updateLMGibbs(RNG *rng) 
    {
	// Update element-wise. Less efficient than updateLM but
	// does not require a Cholesky decomposition, and is 
	// necessary for truncated parameters

	if (_init) {
	    if (_view->length() != _sub_views.size()) {
		throwLogicError("updateLMGibbs can only act on scalar nodes");
	    }
	    calDesign();
	    _init = false;
	}

	double *b = 0;
	cholmod_sparse *A = 0;
	calCoef(b, A);

	int nrow = _view->length();
	vector<double> theta(nrow);
	_view->getValue(theta, _chain);

	int *Ap = static_cast<int*>(A->p);
	int *Ai = static_cast<int*>(A->i);
	double *Ax = static_cast<double*>(A->x);

	//Extract diagonal from A
	vector<double> diagA(nrow);
	for (int c = 0; c < nrow; ++c) {
	    for (int j = Ap[c]; j < Ap[c+1]; ++j) {
		if (Ai[j] == c) {
		    diagA[c] = Ax[j];
		    break;
		}
	    }
	}

	//Update element-wise
	for (int i = 0; i < nrow; ++i) {
	    
	    double theta_old = theta[i];
		
	    double mu  = theta[i] + b[i]/diagA[i];
	    double sigma = sqrt(1/diagA[i]);
	    StochasticNode const *snode = _sub_views[i]->nodes()[0];
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
	    for (int j = Ap[i]; j < Ap[i+1]; ++j) {
		b[Ai[j]] -= delta * Ax[j];
	    }
	}

	_view->setValue(theta,  _chain);
    }

    bool GLMMethod::isAdaptive() const
    {
	return false;
    }
    
    void GLMMethod::adaptOff()
    {
    }

    bool GLMMethod::checkAdaptation() const
    {
	return true;
    }

    void
    GLMMethod::updateAuxiliary(cholmod_dense *b, cholmod_factor *N, RNG *rng)
    {
    }

}}
