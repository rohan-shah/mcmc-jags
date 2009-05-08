#include <config.h>
#include <rng/RNG.h>

//#include "lapack.h"

#include <set>
//#include <stdexcept>
#include <vector>
//#include <cmath>
//#include <string>
#include <stdexcept>

#include "LMSampler.h"

#include <sampler/Linear.h>
#include <graph/StochasticNode.h>
#include <sampler/Sampler.h>

//#include <JRmath.h>

using std::string;
using std::vector;
using std::set;
//using std::sqrt;
//using std::invalid_argument;
using std::logic_error;


void ConjugateLM::calBeta(cs *betas, LMSampler *sampler, unsigned int chain)
{
/*
    vector<StochasticNode *> const &snodes = sampler->nodes();

    vector<double> xnew(_ncol);
    unsigned int k = 0;
    for (unsigned int i = 0; i < snodes.size(); ++i) {
	for (unsigned int j = 0; j < snodes[i]->length(); ++j) {
	    xnew[k++] = snodes[i]->value(chain)[j];
	}
    }

    vector<StochasticNode const*> const &stoch_children = 
        sampler->stochasticChildren();

    double *beta_j = betas->x;
    r = 0;
    for (unsigned int j = 0; j < stoch_children.size(); ++j) {
	double const *mu = stoch_children[j]->parents()[0]->value(chain);
	unsigned int length_child = stoch_children[j]->length();
	for (unsigned int k = 0; k < nrow_child; ++k) {
	    for (unsigned int i = 0; i < nrow; ++i) {
		beta_j[] = -mu[k];
	    }
	}
	beta_j += nrow_child * nrow;
    }

    for (unsigned int i = 0; i < nrow; ++i) {
	xnew[i] += 1;
	sampler->setValue(xnew, nrow, chain);
	beta_j = betas;
	for (unsigned int j = 0; j < stoch_children.size(); ++j) {
	    StochasticNode const *snode = stoch_children[j];
	    double const *mu = snode->parents()[0]->value(chain);
	    unsigned int nrow_child = snode->length();
	    for (unsigned int k = 0; k < nrow_child; ++k) {
		beta_j[nrow * k + i] += mu[k];
	    }
	    beta_j += nrow_child * nrow;
	}
	xnew[i] -= 1;
    }
    sampler->setValue(xnew, nrow, chain);

    delete [] xnew;
*/
}

ConjugateLM::ConjugateLM()
    : _betas(0), _nrow(0), _ncol(0)
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
/*
    vector<StochasticNode *> const &snodes = sampler->nodes();
    vector<StochasticNode const *> const &children = 
	sampler->stochasticChildren();

    int nrow = 0;
    for (unsigned int i = 0; i < children.size(); ++i) {
	nrow += children[i]->length();
    }
    int ncol = sampler->length();

    //Row and column indices for sparse matrix representation
    vector<int> cols(ncol + 1);
    vector<int> rows;

    int c = 0; //column counter
    int nz = 0; //running total of number of non-zero entries
    for (unsigned int i = 0; i < snodes.size(); ++i) {

	set<StochasticNode const *> children_i;
	stochasticChildren(vector<StochasticNode*>(1,snodes[i]), graph,
			   children_i);
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
    int *Ti = T->i;
    int *Tj = T->p;
    double *Tx = T->x;
    for (unsigned int i = 0; i < 

    // Check for constant linear terms
    if (!checkLinear(sampler->nodes(), graph, true))
	return; //Coefficients not fixed

    //Onetime calculation of fixed coefficients
    calBeta(_betas, sampler, 0);
*/
}


void ConjugateLM::update(LMSampler *sampler, unsigned int chain, RNG *rng) const
{
/*
    vector<StochasticNode const*> const &stoch_children = 
          sampler->stochasticChildren();
    unsigned int nchildren = stoch_children.size();
    
    StochasticNode *snode = sampler->node();
    double const *xold = snode->value(chain);
    double const *priormean = snode->parents()[0]->value(chain); 
    double const *priorprec = snode->parents()[1]->value(chain);
    int nrow = snode->length();

    //   The log of the full conditional density takes the form
    //   -1/2(t(x) %*% A %*% x - 2 * b %*% x)

    //   For computational convenience, we reset the origin to xold,
    //   the current value of the node.

    int N = nrow * nrow;
    double *b = new double[nrow];
    double *A = new double[N];
    for (int i = 0; i < nrow; ++i) {
	b[i] = 0;
	for (int i2 = 0; i2 < nrow; ++i2) {
	    b[i] += priorprec[i * nrow + i2] * (priormean[i2] - xold[i2]);
	}
    }
    for (int i = 0; i < N; ++i) {
	A[i] = priorprec[i];
    }
    
    // FORTRAN routines are all call-by-reference, so we need to create
    // these constants
    double zero = 0;
    double d1 = 1;
    int i1 = 1;

    if (sampler->deterministicChildren().empty()) {
      
	// This can only happen if the stochastic children are all
	// multivariate normal with the same number of rows and 
	// columns. We know alpha = 0, beta = I.

	double *delta = new double[nrow]; 

	for (unsigned int j = 0; j < nchildren; ++j) {
	    double const *Y = stoch_children[j]->value(chain);
	    double const *tau = stoch_children[j]->parents()[1]->value(chain);
	    double alpha = 1;
	
	    F77_DAXPY (&N, &alpha, tau, &i1, A, &i1);
	    for (unsigned int i = 0; i < nrow; ++i) {
		delta[i] = Y[i] - xold[i];
	    }
	    F77_DGEMV ("N", &nrow, &nrow, &alpha, tau, &nrow, delta, &i1,
		       &d1, b, &i1);
	}

	delete [] delta;
	
    }
    else {
	
	bool temp_beta = (_betas == 0);
        double *betas = 0;
	if (temp_beta) {
	    betas = new double[_length_betas];
	    calBeta(betas, sampler, chain);
	}
        else {
            betas = _betas;
        }

	//Calculate largest possible size of working matrix C
	int max_nrow_child = 0;
	for (unsigned int j = 0; j < nchildren; ++j) {
	    if (snode->length() > max_nrow_child) {
		max_nrow_child = snode->length();
	    }
	}
	double *C = new double[nrow * max_nrow_child];
	double *delta = new double[max_nrow_child];
	
	//Now add the contribution of each term to A, b 
	//   
	//   b += N_j * beta_j %*% tau_j (Y_j - mu_j)
	//   A += N_j * beta_j %*% tau_j %*% t(beta_j)

	//   where 
	//   - N_j is the frequency weight of child j
	//   - beta_j is a matrix of linear coefficients
	//   - tau_j is the variance-covariance matrix of child j
	//   - mu_j is the mean of child j
	//   - Y_j is the value of child j
	   
	//   We make use of BLAS routines for efficiency.

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
