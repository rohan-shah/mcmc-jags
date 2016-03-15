#include <config.h>

#include "HolmesHeld.h"
#include "KS.h"
#include "Outcome.h"

#include <graph/StochasticNode.h>
#include <graph/LinkNode.h>
#include <sampler/GraphView.h>
#include <rng/TruncatedNormal.h>
#include <rng/RNG.h>
#include <module/ModuleError.h>

#include <cmath>

// Regularization penalty for precision 
#define REG_PENALTY 0.001

using std::vector;
using std::string;
using std::sqrt;

extern cholmod_common *glm_wk;

namespace jags {
namespace glm {

    HolmesHeld::HolmesHeld(GraphView const *view,
			   vector<SingletonGraphView const *> const &sub_views,
			   vector<Outcome *> const &outcomes,
			   unsigned int chain)
	: GLMMethod(view, sub_views, outcomes, chain, true)
    {
    }

    void HolmesHeld::updateAuxiliary(cholmod_dense *w, 
				     cholmod_factor *N, RNG *rng)
    {
	/* 
	   In the parent GLMMethod class, the posterior precision of
           the regression parameters is represented by the matrix "A";
           the posterior mean "mu" solves A %*% mu = b.
	   
           In this call, "N" holds the factorization of P %*% A %*% t(P), 
           where P is a permutation matrix.  The factorization takes
           the form L %*% D %*% t(L), where D is diagonal and L is
           a lower triangular matrix. The parameter "w" solves
           L %*% w = P %*% b
	   
	   IMPORTANT NOTE: mu, b use a parameterization in which the
	   current value of the regression parameters is taken as the
	   origin.  This requires us to adjust the calculations using
	   the current value of the linear predictor (mu_r).
	*/

	vector<StochasticNode *> const &schildren = 
	    _view->stochasticChildren();

	int nrow = schildren.size();

	//Transpose and permute the design matrix
	cholmod_sparse *t_x = cholmod_transpose(_x, 1, glm_wk);
	int *fperm = static_cast<int *>(_factor->Perm);
	cholmod_sparse *Pt_x = cholmod_submatrix(t_x, fperm, t_x->nrow,
						 0, -1, 1, 1, glm_wk);
	cholmod_free_sparse(&t_x, glm_wk);

	int ncol = _x->ncol;
	vector<double> d(ncol, 1);
	if (!_factor->is_ll) {
	    // LDL' decomposition. The diagonal D matrix is stored as
	    // the diagonal of _factor
	    int *fp = static_cast<int*>(_factor->p);
	    double *fx = static_cast<double*>(_factor->x);
	    for (int r = 0; r < ncol; ++r) {
		d[r] = fx[fp[r]];
	    }
	}

	double *wx = static_cast<double *>(w->x);
	
	cholmod_dense *U = 0, *Y = 0, *E = 0;
	cholmod_sparse *Uset = 0;

	cholmod_dense *X = cholmod_allocate_dense(ncol, 1, ncol,
						  CHOLMOD_REAL, glm_wk);
	double *Xx = static_cast<double*>(X->x);
	
	for (int r = 0; r < nrow; ++r) {

	    if (_outcomes[r]->fixedb()) continue;

	    //Get row r of design matrix
	    cholmod_sparse *Xset = cholmod_submatrix(Pt_x, NULL, -1,
						     &r, 1, 1, 1, glm_wk);

	    double *xsx = static_cast<double*>(Xset->x);
	    int *xsp = static_cast<int*>(Xset->p);
	    int *xsi = static_cast<int*>(Xset->i);

	    for (int j = 0; j < xsp[1]; ++j) {
		int c = xsi[j];
		Xx[c] = xsx[j];
	    }
	    
	    cholmod_solve2(CHOLMOD_L, _factor, X, Xset, &U, &Uset, &Y, &E,
			   glm_wk);

	    double mu_r = _outcomes[r]->mean(); // See IMPORTANT NOTE above
	    double tau_r = _outcomes[r]->precision();
	    
	    //Calculate mean and precision of z[r] conditional
	    //on z[s] for s != r
	    double zr_mean = 0;
	    double gr = 0; 

	    int *up = static_cast<int*>(Uset->p);
	    int *ui = static_cast<int*>(Uset->i);
	    double *ux = static_cast<double*>(U->x);

	    for (int j = 0; j < up[1]; ++j) {
		int c = ui[j];
		zr_mean  += ux[c] * wx[c] / d[c]; 
		gr  += ux[c] * ux[c] / d[c];
	    }

	    double Hr = gr * tau_r; // diagonal of hat matrix
	    if (1 - Hr <= 0) {
		// Theoretically this should never happen, but
		// numerical instability may cause it
		continue;
	    }
	    zr_mean -= Hr * (_outcomes[r]->value() - mu_r);
	    zr_mean /= (1 - Hr);
	    double zr_var = gr / (1 - Hr);
	    
	    double zold = _outcomes[r]->value();
	    _outcomes[r]->update(zr_mean + mu_r, zr_var, rng); 
	    
	    //Add new contribution of row r back to b
	    double zdelta = (_outcomes[r]->value() - zold) * tau_r;
	    for (int j = 0; j < up[1]; ++j) {
		int c = ui[j];
		wx[c] += ux[c] * zdelta; 
	    }

	    cholmod_free_sparse(&Xset, glm_wk);
	}
	    
	//Free workspace

	cholmod_free_sparse(&Pt_x, glm_wk);
	cholmod_free_sparse(&Uset, glm_wk);
	
	cholmod_free_dense(&U, glm_wk);
	cholmod_free_dense(&Y, glm_wk);
	cholmod_free_dense(&E, glm_wk);
	cholmod_free_dense(&X, glm_wk);
    }
    
    void HolmesHeld::update(RNG *rng)
    {
	for (vector<Outcome*>::const_iterator p = _outcomes.begin();
	     p != _outcomes.end(); ++p)
	{
	    (*p)->update(rng);
	}
	updateLM(rng);
    }

}}
    
