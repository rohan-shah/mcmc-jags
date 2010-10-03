#include <config.h>

#include "HolmesHeldB.h"
#include "KS.h"

#include <graph/StochasticNode.h>
#include <graph/LinkNode.h>
#include <sampler/GraphView.h>
#include <rng/TruncatedNormal.h>
#include <rng/RNG.h>

#include <stdexcept>
#include <algorithm>

using std::vector;
using std::string;
using std::logic_error;
using std::runtime_error;
using std::copy;

/* Modified version of cs_updown:
   - Sigma can be an arbitrary value instead of just (-1,+1)
   - We can take the rth column of C (counting from 0) for the up/downdate 
     instead of column 0
*/

static int jags_updown (cs *L, double sigma, const cs *C, int r, 
			const int *parent)
{
    int n, p, f, j, *Lp, *Li, *Cp, *Ci ;
    double *Lx, *Cx, alpha, beta = 1, delta, gamma, w1, w2, *w, beta2 = 1 ;
    int sign = (sigma > 0) ? 1 : -1;
    double sigmart = sqrt(fabs(sigma));
    if (!CS_CSC (L) || !CS_CSC (C) || !parent) return (0) ;  // check inputs 
    Lp = L->p ; Li = L->i ; Lx = L->x ; n = L->n ;
    Cp = C->p ; Ci = C->i ; Cx = C->x ;
    if ((p = Cp [r]) >= Cp [r+1]) return (1) ;      // return if C empty
    w = (double*) cs_malloc (n, sizeof (double)) ;  // get workspace 
    if (!w) return (0) ;                            // out of memory 
    f = Ci [p] ;
    for ( ; p < Cp [r+1] ; p++) f = CS_MIN (f, Ci [p]) ; //f = min (find (C)) 
    for (j = f ; j != -1 ; j = parent [j]) w [j] = 0 ;  // clear workspace w 
    for (p = Cp [r] ; p < Cp [r+1] ; p++) w [Ci [p]] = sigmart * Cx [p] ; 
    for (j = f ; j != -1 ; j = parent [j])          // walk path f up to root
    {
        p = Lp [j] ;
        alpha = w [j] / Lx [p] ; // alpha = w(j) / L(j,j) 
        beta2 = beta*beta + sign*alpha*alpha ;
        if (beta2 <= 0) break ;                     // not positive definite
        beta2 = sqrt (beta2) ;
        delta = (sign > 0) ? (beta / beta2) : (beta2 / beta) ;
        gamma = sign * alpha / (beta2 * beta) ;
        Lx [p] = delta * Lx [p] + ((sign > 0) ? (gamma * w [j]) : 0) ;
        beta = beta2 ;
        for (p++ ; p < Lp [j+1] ; p++)
        {
            w1 = w [Li [p]] ;
            w [Li [p]] = w2 = w1 - alpha * Lx [p] ;
            Lx [p] = delta * Lx [p] + gamma * ((sign > 0) ? w1 : w2) ;
        }
    }
    cs_free (w) ;
    return (beta2 > 0) ;
}

namespace glm {

    HolmesHeldB::HolmesHeldB(GraphView const *view,
			   vector<GraphView const *> const &sub_views,
			   unsigned int chain)
	: BinaryGLM(view, sub_views, chain), 
	  _z1(view->stochasticChildren().size(), 0)
    {
    }

    void HolmesHeldB::updateAuxiliary(double *b, csn *N, RNG *rng)
    {
	/* 
	   In the parent GLMMethod class, the posterior precision is
	   represented by the matrix "A"; the posterior mean "mu"
	   solves A %*% mu = b.
	*/

	vector<StochasticNode const *> const &schildren = 
	    _view->stochasticChildren();

	unsigned int nrow = schildren.size();
	unsigned int ncol = _view->length();

	//Transpose and permute the design matrix
	cs *t_x = cs_transpose(_x, 1);
	cs *Pt_x = cs_permute(t_x, _symbol->pinv, 0, 1);
	cs_spfree(t_x);

	//Workspace
	double *ur = new double[ncol];
	int *xi = new int[2*ncol]; //Stack
	double *vr = new double[ncol];

	int *pp = Pt_x->p;
	int *pi = Pt_x->i;
	double *px = Pt_x->x;

	for (unsigned int r = 0; r < nrow; ++r) {

	    if (_outcome[r] == BGLM_LOGIT || _outcome[r] == BGLM_PROBIT)
	    {
		double mu_r = getMean(r);
		
		//Downdate contribution from observation r
		if(!jags_updown(N->L, -_tau[r], Pt_x, r, _symbol->parent)) {
		    throw runtime_error("Downdate error in HolmesHeldB");
		}
		for (unsigned int j = pp[r]; j < pp[r+1]; ++j) {
		    b[pi[j]] -=  _tau[r] * (_z[r] - mu_r) * px[j];
		}

		//Calculate mean and precision of z[r] conditional
		//on z[s] for s != r
		double v2 = 0;
		int top = cs_spsolve(N->L, Pt_x, r, xi, ur, 0, 1);
		for (unsigned int j = top; j < ncol; ++j) {
		    v2  += ur[xi[j]] * ur[xi[j]];
		}

		copy (b, b + ncol, vr);
		cs_lsolve(N->L, vr); //Too slow!

		double zr_mean = mu_r;
		for (unsigned int j = top; j < ncol; ++j) {
		    zr_mean += vr[xi[j]] * ur[xi[j]];
		}

		double yr = schildren[r]->value(_chain)[0];
		double zr_old = _z[r];

		if (_outcome[r] == BGLM_LOGIT) {
		    _z1[r] -= zr_mean;
		    double z2 = _z[r] - _z1[r];
		    _tau[r] = 1/sample_lambda(fabs(_z1[r]), rng);
		    if (yr == 1) {
			_z1[r] = lnormal(-z2, rng, 0, sqrt(1/_tau[r]));
			z2 = lnormal(-_z1[r], rng, zr_mean, sqrt(v2));
			_z1[r] = lnormal(-z2, rng, 0, sqrt(1/_tau[r]));
			
		    }
		    else if (yr == 0) {
			_z1[r] = rnormal(-z2, rng, 0, sqrt(1/_tau[r]));
			z2 = rnormal(-_z1[r], rng, zr_mean, sqrt(v2));
			_z1[r] = rnormal(-z2, rng, 0, sqrt(1/_tau[r]));
		    }
		    else {
			throw logic_error("Invalid child value in HolmesHeldB");
		    }
		    _tau[r] = 1/sample_lambda(fabs(_z1[r]), rng);
		    _z[r] = _z1[r] + z2;
		    _z1[r] += zr_mean;
		}
		else {
		    if (yr == 1) {
			_z[r] = lnormal(0, rng, zr_mean, sqrt(1 + v2));
		    }
		    else if (yr == 0) {
			_z[r] = rnormal(0, rng, zr_mean, sqrt(1 + v2));
		    }
		    else {
			throw logic_error("Invalid child value in HolmesHeldB");
		    }
		}
		//Update contribution from observation r
		if(!jags_updown(N->L, _tau[r], Pt_x, r, _symbol->parent)) {
		    throw runtime_error("Update error in HolmesHeldB");
		}
		for (unsigned int j = pp[r]; j < pp[r+1]; ++j) {
		    b[pi[j]] +=  _tau[r] * (_z[r] - mu_r) * px[j];
		}
	    }
	}

	//Free workspace
	delete [] ur;
	delete [] vr;
	delete [] xi;
	cs_spfree(Pt_x);
    }
    
    void HolmesHeldB::update(RNG *rng)
    {
	updateLM(rng, true, false);
    }

    string HolmesHeldB::name() const
    {
	return "Holmes-HeldB";
    }
}
    
