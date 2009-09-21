#include <config.h>
#include <rng/RNG.h>

#include <set>
#include <vector>
#include <stdexcept>
#include <algorithm>

#include "GLMMethod.h"
#include "GLMSampler.h"

#include <sampler/Linear.h>
#include <graph/StochasticNode.h>
#include <graph/DeterministicNode.h>
#include <sampler/Sampler.h>

using std::string;
using std::vector;
using std::set;
using std::logic_error;
using std::copy;

static void updateDeterministic(Node *node, unsigned int chain)
{
    set<DeterministicNode*> const *dch = node->deterministicChildren();
    for (set<DeterministicNode*>::const_iterator p = dch->begin();
	 p != dch->end(); ++p)
    {
	(*p)->deterministicSample(chain);
	updateDeterministic(*p, chain);
    }
}

namespace glm {

    void GLMMethod::calBeta(cs *beta, GLMSampler *sampler, unsigned int chain)
	const
    {
	vector<StochasticNode *> const &snodes = sampler->nodes();
	vector<StochasticNode const *> const &schildren = 
	    sampler->stochasticChildren();

	int *Bi = beta->i;
	int *Bp = beta->p;
	double *Bx = beta->x;

	int nrow = schildren.size();
	int ncol = sampler->length();
	if (nrow != beta->m || ncol != beta->n) {
	    throw logic_error("Dimension mismatch in GLMMethod::calBeta");
	}

	for (unsigned int r = 0; r < Bp[ncol]; ++r) {
	    Bx[r] = -getMean(schildren, Bi[r], chain);
	}

	int c = 0; //column counter
	double *xnew = new double[_length_max];
	for (vector<StochasticNode*>::const_iterator p = snodes.begin();
	     p != snodes.end(); ++p)
	{
	    Node *snode = *p;
	    double const *xold = snode->value(chain);
	    unsigned int length = snode->length();
	
	    copy(xold, xold + length, xnew);
	    for (unsigned int i = 0; i < length; ++i, ++c) {

		xnew[i] += 1;
		snode->setValue(xnew, length, chain);
		updateDeterministic(snode, chain); 
				
		for (int r = Bp[c]; r < Bp[c + 1]; ++r) {
		    Bx[r] += getMean(schildren, Bi[r], chain);
		}

		xnew[i] -= 1;
	    }
	    snode->setValue(xnew, length, chain);
	    updateDeterministic(snode, chain);
	}
	delete [] xnew;
    
    }
    
    GLMMethod::GLMMethod()
	: _beta(0), _symbol(0), _fixed(false), _length_max(0), _nz_prior(0)
    {
    }

    GLMMethod::~GLMMethod()
    {
	cs_spfree(_beta);
    }

    static void getIndices(set<StochasticNode const *> const &schildren,
			   vector<StochasticNode const*> const &rows,
			   vector<int> &indices)
    {
	indices.clear();

	for (int i = 0; i < rows.size(); ++i) {
	    if (schildren.count(rows[i])) {
		indices.push_back(i);
	    }
	}
	
	if (indices.size() != schildren.size()) {
	    throw logic_error("Size mismatch in getIndices");
	}
    }
    
/* 
   Symbolic analysis of the posterior precision matrix for the
   Cholesky decomposition.

   This only needs to be done once, at initialization.  It is a
   stripped-down version of the code in update.  Note that the values
   of the sparse matrices are never referenced.
*/
    void GLMMethod::symbolic(GLMSampler const *sampler)  
    {
	vector<StochasticNode const*> const &schildren = 
	    sampler->stochasticChildren();
	unsigned int nchildren = schildren.size();
    
	unsigned int nrow = sampler->length();
	cs *Aprior = cs_spalloc(nrow, nrow, _nz_prior, 0, 0); 
    
	// Prior contribution
	int *Ap = Aprior->p;
	int *Ai = Aprior->i;

	int c = 0;
	int r = 0;
	vector<StochasticNode*> const &snodes = sampler->nodes();
	for (vector<StochasticNode*>::const_iterator p = snodes.begin();
	     p != snodes.end(); ++p)
	{
	    StochasticNode *snode = *p;
	    int length = snode->length();

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
    
	cs *tbeta = cs_transpose(_beta, 0);
	cs *Alik = cs_multiply(tbeta, _beta);
	cs *A = cs_add(Aprior, Alik, 0, 0);
    
	//Free working matrices
	cs_spfree(tbeta);
	cs_spfree(Aprior);
	cs_spfree(Alik);

	_symbol = cs_schol(1, A); 
	cs_spfree(A);
    }

    void GLMMethod::initialize(GLMSampler *sampler, Graph const &graph)
    {
	vector<StochasticNode *> const &snodes = sampler->nodes();
	vector<StochasticNode const*> const &schildren = 
	    sampler->stochasticChildren();

	int nrow = schildren.size();
	int ncol = sampler->length();

	vector<int> Bp(ncol + 1);
	vector<int> Bi;
    
	int c = 0; //column counter
	int r = 0; //count of number of non-zero entries
	for (vector<StochasticNode*>::const_iterator p = snodes.begin();
	     p != snodes.end(); ++p)
	{
	    StochasticNode *snode = *p;

	    set<StochasticNode const *> children_p;
	    Sampler::getStochasticChildren(vector<StochasticNode*>(1,snode), 
					   graph, children_p);
	    vector<int> indices;
	    getIndices(children_p, schildren, indices);

	    unsigned int length = snode->length();
	    for (unsigned int i = 0; i < length; ++i, ++c) {
		Bp[c] = r;
		for (unsigned int j = 0; j < indices.size(); ++j, ++r) {
		    Bi.push_back(indices[j]);
		}
	    }

	    //Save these values for later calculations
	    _nz_prior += length * length; //Number of non-zeros in prior precision
	    if (length > _length_max) {
		_length_max = length; //Length of longest sampled node
	    }
	}
	Bp[c] = r;

	//Set up sparse matrix representation of beta coefficients
	_beta = cs_spalloc(nrow, ncol, r, 1, 0);
	copy(Bp.begin(), Bp.end(), _beta->p);
	copy(Bi.begin(), Bi.end(), _beta->i);

	//Symbolic analysis
	calBeta(_beta, sampler, 0);
	symbolic(sampler);

	// Check for constant linear terms
	if (checkLinear(sampler->nodes(), graph, true)) {
	    _fixed = true;
	}
    }


    void 
    GLMMethod::update(GLMSampler *sampler, unsigned int chain, RNG *rng) const
    {
	vector<StochasticNode const *> const &schildren =
	    sampler->stochasticChildren();
	unsigned int nchildren = schildren.size();
    
	//   The log of the full conditional density takes the form
	//   -(t(x) %*% A %*% x - 2 * b %*% x)/2
	//   where A is the posterior precision and the mean mu solves
	//   A %*% mu = b

	//   For computational convenience we take xold, the current value
	//   of the sampled nodes, as the origin

	unsigned int nrow = sampler->length();
	double *b = new double[nrow];
	cs *Aprior = cs_spalloc(nrow, nrow, _nz_prior, 1, 0); 
    
	// Set up prior contributions to A, b
	int *Ap = Aprior->p;
	int *Ai = Aprior->i;
	double *Ax = Aprior->x;

	int c = 0;
	int r = 0;
	vector<StochasticNode*> const &snodes = sampler->nodes();
	for (vector<StochasticNode*>::const_iterator p = snodes.begin();
	     p != snodes.end(); ++p)
	{
	    StochasticNode *snode = *p;
	    double const *priormean = snode->parents()[0]->value(chain);
	    double const *priorprec = snode->parents()[1]->value(chain);
	    double const *xold = snode->value(chain);
	    int length = snode->length();
	
	    int cbase = c; //first column of this diagonal block
	    for (unsigned int i = 0; i < length; ++i, ++c) {
		b[c] = 0;
		Ap[c] = r;
		for (unsigned int j = 0; j < length; ++j, ++r) {
		    b[c] += priorprec[i + length * j] * (priormean[j] - xold[j]);
		    Ai[r] = cbase + j;
		    Ax[r] = priorprec[i + length * j];
		}
	    }
	}
	Ap[c] = r;

	if (!_fixed) {
	    //Fixme: could be more efficient if we don't have to recalculate
	    //all coefficients
	    calBeta(_beta, sampler, chain);
	}

	// Likelihood contributions
	//   
	//   b += t(beta) %*% tau %*% (Y - mu)
	//   A += t(beta) %*% tau %*% beta
    
	//   where 
	//   - beta is a matrix of linear coefficients
	//   - tau is the (diagonal) variance matrix of the stochastic children
	//   - mu is the mean of the stochastic children
	//   - Y is the value of the stochastic children

	cs *tbeta = cs_transpose(_beta, 1);
	int *Tp = tbeta->p;
	int *Ti = tbeta->i;
	double *Tx = tbeta->x;
    
	for (int c = 0; c < tbeta->n; ++c) {
	    double tau = getPrecision(schildren, c, chain);
	    double delta = getValue(schildren, c, chain) - 
		getMean(schildren, c, chain);
	    for (int r = Tp[c]; r < Tp[c+1]; ++r) {
		Tx[r] *= tau;
		b[Ti[r]] += Tx[r] * delta;
	    
	    }
	}

	cs *Alik = cs_multiply(tbeta, _beta);
	cs_spfree(tbeta);
	cs *A = cs_add(Aprior, Alik, 1, 1);
	cs_spfree(Aprior);
	cs_spfree(Alik);

	// Get Cholesky decomposition of posterior precision
	csn *N = cs_chol(A, _symbol);
	if (!N) {
	    throw logic_error("Cholesky decomposition failure in GLMMethod");
	}
	cs_spfree(A);

	// Use the Cholesky decomposition to generate a new sample
	// with mean mu such that A %*% mu = b and precision A. The
	// vector b is overwritten with the result

	double *w = new double[nrow];
	cs_ipvec(_symbol->pinv, b, w, nrow);
	cs_lsolve(N->L, w);
	for (r = 0; r < nrow; ++r) {
	    w[r] += rng->normal();
	}
	cs_ltsolve(N->L, w);
	cs_nfree(N);
	cs_pvec(_symbol->pinv, w, b, nrow);
	delete [] w;
    
	//Shift origin back to original scale
	r = 0;
	for (vector<StochasticNode*>::const_iterator p = snodes.begin();
	     p != snodes.end(); ++p)
	{
	    StochasticNode *snode = *p;
	    unsigned int length = snode->length();
	    double const *xold = snode->value(chain);
	    for (unsigned int i = 0; i < length; ++i, ++r) {
		b[r] += xold[i];
	    }
	}

	sampler->setValue(b, nrow, chain);
	delete [] b;
    }


}
