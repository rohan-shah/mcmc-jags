#include <config.h>
#include <rng/RNG.h>

//#include "lapack.h"

#include <set>
//#include <stdexcept>
#include <vector>
//#include <cmath>
//#include <string>
#include <stdexcept>
#include <algorithm>

#include "LMSampler.h"

#include <sampler/Linear.h>
#include <graph/StochasticNode.h>
#include <graph/DeterministicNode.h>
#include <sampler/Sampler.h>

//#include <JRmath.h>

using std::string;
using std::vector;
using std::set;
//using std::sqrt;
//using std::invalid_argument;
using std::logic_error;
using std::copy;


static double getMean(StochasticNode const *snode, unsigned int chain) {
    return snode->parents()[0]->value(chain)[0];
}

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

void ConjugateLM::calBeta(cs *betas, LMSampler *sampler, unsigned int chain)
const
{
    vector<StochasticNode *> const &snodes = sampler->nodes();
    vector<StochasticNode const*> const &schildren = 
	sampler->stochasticChildren();

    int *Bi = betas->i;
    int *Bp = betas->p;
    double *Bx = betas->x;

    int nrow = betas->m;
    int nz = betas->nz;

    for (unsigned int z = 0; z < nz; ++z) {
	Bx[z] = -getMean(schildren[Bi[z]], chain);
    }

    int c = 0; //column counter
    unsigned int length_max = 0;
    double *xnew = 0;
    for (vector<StochasticNode*>::const_iterator p = snodes.begin();
	 p != snodes.end(); ++p)
    {
	Node *snode = *p;
	unsigned int length = snode->length();
	if (length > length_max) {
	    delete [] xnew;
	    xnew = new double[length];
	    length_max = length;
	}
	double const *xold = snode->value(chain);
	copy(xold, xold + nrow, xnew);
	for (unsigned int i = 0; i < length; ++i) {
	    xnew[i] += 1;
	    snode->setValue(xnew, length, chain);
	    updateDeterministic(snode, chain); 
				
	    for (int z = Bp[c]; z < Bp[c + 1]; ++z) {
		Bx[z] += getMean(schildren[Bi[z]], chain);
	    }
	    xnew[i] -= 1;
	    ++c;
	}
	snode->setValue(xnew, nrow, chain);
	updateDeterministic(snode, chain);
    }
    delete [] xnew;
	
}

ConjugateLM::ConjugateLM()
    : _betas(0), _fixed(false)
{
    
}

ConjugateLM::~ConjugateLM()
{
    cs_free(_betas);
}

static void getIndices(set<StochasticNode const*> const &schildren,
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

void ConjugateLM::initialize(LMSampler *sampler, Graph const &graph)
{
    vector<StochasticNode *> const &snodes = sampler->nodes();
    vector<StochasticNode const *> const &children = 
	sampler->stochasticChildren();

    int nrow = children.size();
    int ncol = sampler->length();

    //Row and column indices for sparse matrix representation
    vector<int> cols(ncol + 1);
    vector<int> rows;

    int c = 0; //column counter
    int nz = 0; //running total of number of non-zero entries
    for (unsigned int i = 0; i < snodes.size(); ++i) {

	set<StochasticNode const *> children_i;
	Sampler::getStochasticChildren(vector<StochasticNode*>(1,snodes[i]), 
				      graph, children_i);
	vector<int> indices;
	getIndices(children_i, children, indices);

	for (unsigned int j = 0; j < snodes[i]->length(); ++j) {
	    cols[c++] = nz;
	    for (unsigned int k = 0; k < indices.size(); ++k) {
		rows.push_back(indices[k]);
	    }
	    nz += indices.size();
	}
    }
    cols[c] = nz;


    //Set up sparse matrix representation of beta coefficients
    cs *T = cs_spalloc(nrow, ncol, nz, 1, 0);
    int *Tj = T->p;
    for (unsigned int k = 0; k < nz; ++k) {
	T->i[k] = rows[k];
    }
    for (unsigned int j = 0; j <= ncol; ++j) {
	T->p[j] = cols[j];
    }
    T->nz = nz; //Not sure if we need this or not.
    _betas = T;

    // Check for constant linear terms
    if (checkLinear(sampler->nodes(), graph, true)) {
	_fixed = true;
	calBeta(_betas, sampler, 0);
    }
}


void ConjugateLM::update(LMSampler *sampler, unsigned int chain, RNG *rng) const
{
    vector<StochasticNode const*> const &stoch_children = 
          sampler->stochasticChildren();
    unsigned int nchildren = stoch_children.size();
    
    //   The log of the full conditional density takes the form
    //   -(t(x) %*% A %*% x - 2 * b %*% x)/2

    //   For computational convenience, we reset the origin to xold,
    //   the current value of the sampler.

    // The vector b is dense, but the matrsampler->length();ix A has a block diagonal
    // structure. We construct it as a sparse matrix in triplet form
    // then compress.
    unsigned int nrow = sampler->length();
    double *b = new double[nrow];
    cs *A = cs_spalloc(nrow, nrow, 1, 1, 1); //better nzmax 
    
    vector<StochasticNode*> const &snodes = sampler->nodes();
    int c = 0;
    for (unsigned int i = 0; i < snodes.size(); ++i) {

	int length_i = snodes[i]->length();
	double const *priormean = snodes[i]->parents()[0]->value(chain);
	double const *priorprec = snodes[i]->parents()[1]->value(chain);
	
	double const *xold = snodes[i]->value(chain);
	for (int j = 0; j < length_i; ++j) {
	    b[j] = 0;
	    for (int k = 0; k < length_i; ++k) {
		b[j] += priorprec[j * length_i + k] * (priormean[k] - xold[k]);
	    }
	}

	for (int j = 0; j < length_i; ++j) {
	    for (int k = 0; k < length_i; ++k) {
		cs_entry(A, c+j, c+k, priorprec[j + length_i * k]);
	    }
	}
	c += length_i;
    }
    cs_compress(A);

    if (!_fixed) {
	//Fixme: could be more efficient if we don't have to recalculate
	//all coefficients
	calBeta(_betas, sampler, chain);
    }

    /*
    //Calculate largest possible size of working matrix C
    int max_nrow_child = 0;
    for (unsigned int j = 0; j < nchildren; ++j) {
	if (snode->length() > max_nrow_child) {
	    max_nrow_child = snode->length();
	}
    }
    double *C = new double[nrow * max_nrow_child];
    double *delta = new double[max_nrow_child];
    */

    //Now add the contribution of each term to A, b 
    //   
    //   b += beta_j %*% tau_j (Y_j - mu_j)
    //   A += beta_j %*% tau_j %*% t(beta_j)
    
    //   where 
    //   - beta_j is a matrix of linear coefficients
    //   - tau_j is the variance-covariance matrix of child j
    //   - mu_j is the mean of child j
    //   - Y_j is the value of child j
    
    //   We make use of BLAS routines for efficiency.

/*    
    double const *beta_j = betas;
    for (unsigned int j = 0; j < nchildren; ++j) {
	
	StochasticNode const *snode = stoch_children[j];
	double const *Y = snode->value(chain);
	double const *mu = snode->parents()[0]->value(chain);
	double const *tau = snode->parents()[1]->value(chain);
	int nrow_child = snode->length();
	
	if (nrow_child == 1) {
	    
	    double alpha = tau[0];
	    F77_DSYR("L", &nrow, &alpha, beta_j, &i1, A, &nrow);
	    alpha *= (Y[0] - mu[0]);
	    F77_DAXPY(&nrow, &alpha, beta_j, &i1, b, &i1);
	    
	}
	else {
	    
	    double alpha = 1;
	    
	    F77_DSYMM("R", "L", &nrow, &nrow_child, &alpha, tau,
		      &nrow_child, beta_j, &nrow, &zero, C, &nrow);
	    
	    for (unsigned int i = 0; i < nrow_child; ++i) {
		delta[i] = Y[i] - mu[i];
	    }
	    
	    F77_DGEMV("N", &nrow, &nrow_child, &d1, C, &nrow,
		      delta, &i1, &d1, b, &i1);
	    F77_DGEMM("N","T", &nrow, &nrow, &nrow_child,
		      &d1, C, &nrow, beta_j, &nrow, &d1, A, &nrow);
	}
	
	beta_j += nrow_child * nrow;
    }
    
    delete [] C;
    delete [] delta;
    
    if (temp_beta) {
	delete betas;
    }



    // Solve the equation A %*% x = b to get the posterior mean.
    // We have to take a copy of A as it is overwritten during
    // the call to DPOSV. The result is stored in b

    double * Acopy = new double[N];
    for (int i = 0; i < N; ++i) {
	Acopy[i] = A[i];
    }
    int one = 1;
    int info;
    F77_DPOSV ("L", &nrow, &one, Acopy, &nrow, b, &nrow, &info);
    if (info != 0) {
	delete [] Acopy;
	delete [] A;
	delete [] b;
	throw NodeError(snode,
			"unable to solve linear equations in Conjugate mnorm sampler");
    }

    //Shift origin back to original scale
    for (int i = 0; i < nrow; ++i) {
	b[i] += xold[i];
    }
    double *xnew = new double[nrow];
    //FIXME. This must use lower triangle of A!!!!
    DMNorm::randomsample(xnew, b, A, true, nrow, rng);
    sampler->setValue(xnew, nrow, chain);

    delete [] A;
    delete [] Acopy;
    delete [] b;
    delete [] xnew;
*/
}

string ConjugateLM::name() const
{
    return "ConjugateLM";
}
