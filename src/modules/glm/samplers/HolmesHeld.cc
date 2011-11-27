#include <config.h>

#include "HolmesHeld.h"
#include "KS.h"

#include <graph/StochasticNode.h>
#include <graph/LinkNode.h>
#include <sampler/GraphView.h>
#include <rng/TruncatedNormal.h>
#include <rng/RNG.h>
#include <module/ModuleError.h>

extern "C" {
#include <cs.h>
}

#include <cmath>

// Regularization penalty for precision 
#define REG_PENALTY 0.001

using std::vector;
using std::string;
using std::sqrt;

extern cholmod_common *glm_wk;

static cs shallow_copy(cholmod_sparse *s)
{
    //Copy a cholmod_sparse struct to a cs struct without allocating
    //any memory

    cs c;
    c.nzmax = s->nzmax;
    c.n = s->nrow;
    c.m = s->ncol;
    c.p = static_cast<int*>(s->p);
    c.i = static_cast<int*>(s->i);
    c.x = static_cast<double*>(s->x);
    c.nz = -1;
    return c;
}

namespace glm {

    HolmesHeld::HolmesHeld(GraphView const *view,
			   vector<GraphView const *> const &sub_views,
			   unsigned int chain)
	: BinaryGLM(view, sub_views, chain), _aux_init(true)
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
	   current value of the regressions parameters is taken as the
	   origin.  This requires us to adjust the calculations using
	   the current value of the linear predictor (mu_r).
	*/

	vector<StochasticNode const *> const &schildren = 
	    _view->stochasticChildren();

	unsigned int nrow = schildren.size();

	//Transpose and permute the design matrix
	cholmod_sparse *t_x = cholmod_transpose(_x, 1, glm_wk);
	int *fperm = static_cast<int *>(_factor->Perm);
	cholmod_sparse *Pt_x = cholmod_submatrix(t_x, fperm, t_x->nrow,
						 0, -1, 1, 1, glm_wk);
	cholmod_free_sparse(&t_x, glm_wk);

	//Turn the factor into a matrix. Since this modifies the factor
	//we need to take a copy first
	cholmod_factor *f = cholmod_copy_factor(_factor, glm_wk);
	cholmod_sparse *L = cholmod_factor_to_sparse(f, glm_wk);
	if (!L->packed || !L->sorted) {
	    throwLogicError("Cholesky factor is not packed or not sorted");
	}
	cholmod_free_factor(&f, glm_wk);

	unsigned int ncol = L->ncol;
	vector<double> d(ncol, 1);
	if (!_factor->is_ll) {
	    //Extract D from the diagonal of L and set the diagonal to 1
	    int *Lp = static_cast<int*>(L->p);
	    double *Lx = static_cast<double*>(L->x);
	    for (unsigned int r = 0; r < ncol; ++r) {
		d[r] = Lx[Lp[r]];
		Lx[Lp[r]] = 1;
	    }
	}

	// Now make shallow copies of L and Pt_x so we can pass them
	// to cs_sparse from the CSparse library

	cs cs_L = shallow_copy(L);
	cs cs_Ptx = shallow_copy(Pt_x);

        double *ur = new double[ncol];
        int *xi = new int[2*ncol]; //Stack
	
	double *wx = static_cast<double *>(w->x);
	
	for (unsigned int r = 0; r < nrow; ++r) {

	    // In a heterogeneous GLM, we may have some normal
	    // outcomes as well as binary outcomes. These can be
	    // skipped as there is no need for auxiliary variables
	    if (_outcome[r] == BGLM_NORMAL)
		continue;
		
	    int top = cs_spsolve(&cs_L, &cs_Ptx, r, xi, ur, 0, 1);
		
	    double mu_r = getMean(r); // See IMPORTANT NOTE above
	    double tau_r = getPrecision(r);
	    
	    //Calculate mean and precision of z[r] conditional
	    //on z[s] for s != r
	    double zr_mean = 0;
	    double Hr = 0; // Diagonal of hat matrix
	    
	    if (_factor->is_ll) {
		for (unsigned int j = top; j < ncol; ++j) {
		    zr_mean  += ur[xi[j]] * wx[xi[j]]; 
		    Hr  += ur[xi[j]] * ur[xi[j]];
		}
	    }
	    else {
		for (unsigned int j = top; j < ncol; ++j) {
		    zr_mean  += ur[xi[j]] * wx[xi[j]] / d[xi[j]]; 
		    Hr  += ur[xi[j]] * ur[xi[j]] / d[xi[j]];
		}
	    }
	    Hr *= tau_r;
	    if (1 - Hr <= 0) {
		// Theoretically this should never happen, but numerical instability may cause it
		StochasticNode const *snode = _view->stochasticChildren()[r];
		throwNodeError(snode, "Highly influential outcome variable in Holmes-Held update method.");
	    }
	    zr_mean -= Hr * (_z[r] - mu_r);
	    zr_mean /= (1 - Hr);
	    double zr_prec = (1 - Hr) * tau_r;
	    
	    double yr = schildren[r]->value(_chain)[0];
	    double zold = _z[r];
	    if (yr == 1) {
		_z[r] = lnormal(0, rng, mu_r + zr_mean, 1/sqrt(zr_prec));
	    }
	    else if (yr == 0) {
		_z[r] = rnormal(0, rng, mu_r + zr_mean, 1/sqrt(zr_prec));
	    }
	    else {
		throwLogicError("Invalid child value in HolmesHeld");
	    }
	    
	    //Add new contribution of row r back to b
	    double zdelta = (_z[r] - zold) * tau_r;
	    for (unsigned int j = top; j < ncol; ++j) {
		wx[xi[j]] += ur[xi[j]] * zdelta; 
	    }
	}
	    
	//Free workspace
	delete [] ur;
	delete [] xi;
	
	cholmod_free_sparse(&Pt_x, glm_wk);
	cholmod_free_sparse(&L, glm_wk);
    }
    
    void HolmesHeld::update(RNG *rng)
    {
	if (_aux_init) {
	    initAuxiliary(rng);
	    _aux_init = false;
	}

	for (unsigned int r = 0; r < _tau.size(); ++r)
	{
	    if (_outcome[r] == BGLM_LOGIT) {
		double delta = fabs(getValue(r) - getMean(r));
		_tau[r] = REG_PENALTY + 1/sample_lambda(delta, rng);
	    }
	}

	updateLM(rng);
    }

    string HolmesHeld::name() const
    {
	return "Holmes-Held";
    }
}
    
