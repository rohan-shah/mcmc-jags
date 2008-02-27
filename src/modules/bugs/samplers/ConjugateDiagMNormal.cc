#include <config.h>
#include <rng/RNG.h>
#include <graph/AggNode.h>
#include <graph/MixtureNode.h>
#include <graph/NodeError.h>
#include <graph/LogicalNode.h>
#include <graph/StochasticNode.h>
#include <sampler/Linear.h>
#include <graph/AggNode.h>

#include "lapack.h"

#include <set>
#include <stdexcept>
#include <vector>
#include <cmath>
#include <string>

#include "ConjugateDiagMNormal.h"
#include "ConjugateSampler.h"
#include "DMNorm.h"

#include <JRmath.h>

using std::string;
using std::vector;
using std::set;
using std::sqrt;
using std::invalid_argument;
using std::string;

static void calBeta(double *betas, ConjugateSampler *sampler,
                    unsigned int chain)
{
    StochasticNode *snode = sampler->node();
    double const *xold = snode->value(chain);
    unsigned int nrow = snode->length();

    double *xnew = new double[nrow];
    for (unsigned int i = 0; i < nrow; ++i) {
	xnew[i] = xold[i];
    }

    vector<StochasticNode const*> const &stoch_children = 
        sampler->stochasticChildren();

    unsigned long nchildren = stoch_children.size();
    double *beta_j = betas;
    for (unsigned int j = 0; j < nchildren; ++j) {
	StochasticNode const *snode = stoch_children[j];
	double const *mu = snode->parents()[0]->value(chain);
	unsigned int nrow_child = snode->length();
	for (unsigned int k = 0; k < nrow_child; ++k) {
	    for (unsigned int i = 0; i < nrow; ++i) {
		beta_j[nrow * k + i] = -mu[k];
	    }
	}
	beta_j += nrow_child * nrow;
    }

    for (unsigned int i = 0; i < nrow; ++i) {
	xnew[i] += 1;
	sampler->setValue(xnew, nrow, chain);
	beta_j = betas;
	for (unsigned int j = 0; j < nchildren; ++j) {
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
}

ConjugateDiagMNormal::ConjugateDiagMNormal()
{
}

ConjugateDiagMNormal::~ConjugateDiagMNormal()
{
}

void ConjugateDiagMNormal::initialize(ConjugateSampler *sampler, 
				      Graph const &graph)
{
    GraphMarks marks(graph);

    unsigned int nchild = stoch_nodes.size();
    for (unsigned int i = 0; i < nchild; ++i) {
	marks.markAncestors(stoch_nodes[i]);
    }

    _offsets.clear();
    _offsets.resize(nchild, 0);

    // Need to exclude non-informative children, so we create a set
    // containing only informative descendants

    set<Node*> paramset;
    for (unsigned int i = 0; i < dtrm_nodes.size(); ++i) {
	localgraph.add(dtrm_nodes[i]);
    }

    set<Node*>  const * children = snode->children();
    for (set<Node*>::const_iterator p = children->begin(); 
	 p != children->end(); ++p)
    {
	if (paramset.count(*p)) {
	    int index = marks.mark(*p);
	    if (index > 0) {
		_offsets[index - 1] = asAggNode(*p)->offsets()[0];
	    }
	}
    }
}

bool ConjugateDiagMNormal::canSample(StochasticNode *snode, Graph const &graph)
{
    //This is a special case of the conjugate normal sampler
    //so if that fails, then so does this
    if (!ConjugateNormal::canSample(snode, graph))
	return false;

    vector<StochasticNode const*> stoch_nodes;
    vector<Node*> dtrm_nodes, extra_nodes;
    Sampler::classifyChildren(vector<StochasticNode*>(1,snode), 
		              graph, stoch_nodes, dtrm_nodes);

    //All stochastic children must be scalar
    for (unsigned int i = 0; i < stoch_nodes.size(); ++i) {
	if (stoch_node[i]->length() != 1)
	    return false;
    }

    /* 
       Create a set of nodes containing snode and its deterministic
       descendants for the checks below.
    */
    set<Node const *> paramset;
    paramset.insert(snode);
    for (unsigned int j = 0; j < dtrm_nodes.size(); ++j) {
	paramset.insert(dtrm_nodes[j]);
    }

    /* Test children of snode to see if they are distinct scalar subsets */
    unsigned int N = snode->size();
    vector<AggNode const *> anodes(N,0);

    unsigned int nsubset = 0;
    set<Node*>  const * children = snode->children();
    for (set<Node*>::const_iterator p = children->begin(); 
	 p != children->end(); ++p)
    {
	if (paramset.count(*p)) {
	    if ((*p)->length() != 1)
		return false; //Not a scalar node
	    AggNode const *a = asAggNode(*p);
	    if (a == 0) 
		return false; //Not a subset of snode
	    unsigned int offset = a->offsets()[0];
	    if (anodes[offset] != 0)
		return false; //Duplicate child node
	    anodes[offset] = a;
	    ++nsubset;
	}
    }

    GraphMarks marks(graph);
    for (unsigned int i = 0; i < N; ++i) {
	if (anodes[i]) {
	    marks.markChildren(anodes[i], i+1);
	}
    }

    set<unsigned int> markset;
    for (unsigned int j = 0; j < _stoch_children.size(); ++j) {
	unsigned int jmark = marks.mark(_stoch_children[j]);
	if (markset.count(jmark) != 0)
	    return false; //Subset has two stochastic children
	markset.insert(jmark);
    }
    
    if (markset.size() != nsubset) {
	//This will happen if a stochastic child has two subset ancestors
	return false;
    }
    
    return true; //We made it!
}

void ConjugateDiagMNormal::update(ConjugateSampler *sampler, unsigned int chain, 
			      RNG *rng) const
{
    vector<StochasticNode const*> const &stoch_children = 
          sampler->stochasticChildren();
    unsigned int nchildren = stoch_children.size();
    
    StochasticNode *snode = sampler->node();
    double const *xold = snode->value(chain);
    double const *priormean = snode->parents()[0]->value(chain); 
    double const *priorprec = snode->parents()[1]->value(chain);
    int nrow = snode->length();
    /* 
       The log of the full conditional density takes the form
       -1/2(t(x) %*% A %*% x - 2 * b %*% x)

       For computational convenience, we reset the origin to xold,
       the current value of the node.
    */
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
    
    /* FORTRAN routines are all call-by-reference, so we need to create
     * these constants */
    double zero = 0;
    double d1 = 1;
    int i1 = 1;
	
    bool temp_beta = (_betas == 0);
    double *betas = new double
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
    
    /* Now add the contribution of each term to A, b 
       
       b += N_j * beta_j %*% tau_j (Y_j - mu_j)
       A += N_j * beta_j %*% tau_j %*% t(beta_j)
       
       where 
       - N_j is the frequency weight of child j
       - beta_j is a matrix of linear coefficients
       - tau_j is the variance-covariance matrix of child j
       - mu_j is the mean of child j
       - Y_j is the value of child j
       
       We make use of BLAS routines for efficiency.
       
    */
    double const *beta_j = betas;
    for (unsigned int j = 0; j < nchildren; ++j) {
	
	StochasticNode const *snode = stoch_children[j];
	double const *Y = snode->value(chain);
	double const *mu = snode->parents()[0]->value(chain);
	double const *tau = snode->parents()[1]->value(chain);
	int nrow_child = snode->length();
	unsigned int Nrep = snode->freqWeight();
	
	if (nrow_child == 1) {
	    
	    double alpha = Nrep * tau[0];
	    F77_DSYR("L", &nrow, &alpha, beta_j, &i1, A, &nrow);
	    alpha *= (Y[0] - mu[0]);
	    F77_DAXPY(&nrow, &alpha, beta_j, &i1, b, &i1);
	    
	}
	else {
	    
	    double alpha = Nrep;
	    
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
	
	/*
	//This turns out to be rather expensive to do by hand
	//for large matrices.
	
	for (int i = 0; i < nrow; ++i) {
	for (int k = 0; k < nrow_child; ++k) {
	
	double beta_tau = 0;
	for (int k2 = 0; k2 < nrow_child; ++k2) {
	beta_tau += 
	beta_j[nrow * k2 + i] * tau[nrow_child * k2 + k];
	}
	
	for (int i2 = 0; i2 < nrow; ++i2) {
	A[i * nrow + i2] += Nrep * 
	beta_tau * beta_j[nrow * k + i2];
	}
	
	b[i] += Nrep * (Y[k] - mu[k]) * beta_tau;
	
	}
	}
	*/
	
	beta_j += nrow_child * nrow;
    }
    
    delete [] C;
    delete [] delta;
    
    if (temp_beta) {
	delete betas;
    }
}


    /* 
       Solve the equation A %*% x = b to get the posterior mean.
       We have to take a copy of A as it is overwritten during
       the call to DPOSV. The result is stored in b
    */
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
}

string ConjugateDiagMNormal::name() const
{
    return "ConjugateDiagMNormal";
}
