#include <config.h>

#include "HolmesHeld.h"
#include "KS.h"

#include <graph/StochasticNode.h>
#include <graph/LinkNode.h>
#include <sampler/GraphView.h>
#include <rng/TruncatedNormal.h>
#include <rng/RNG.h>

#include <stdexcept>

using std::vector;
using std::string;
using std::logic_error;
using std::runtime_error;

namespace glm {

    HolmesHeld::HolmesHeld(GraphView const *view,
			   vector<GraphView const *> const &sub_views,
			   unsigned int chain)
	: BinaryGLM(view, sub_views, chain)
    {
    }

    void HolmesHeld::updateAuxiliary(double *w, csn *N, RNG *rng)
    {
	/* 
	   In the parent GLMMethod class, the posterior precision is
	   represented by the matrix "A"; the posterior mean "mu"
	   solves A %*% mu = b.
	   
	   In this call, "w" solves A %*% w = P %*% b and "N" holds
	   the Cholesky decomposition of P %*% A %*% t(P). 
	*/

	vector<StochasticNode const *> const &schildren = 
	    _view->stochasticChildren();

	unsigned int nrow = schildren.size();
	unsigned int ncol = _view->length();

	//Transpose and permute the design matrix
	cs *t_x = cs_transpose(_x, 1);
	cs *Pt_x = cs_permute(t_x, _symbol->pinv, 0, 1);
	cs_spfree(t_x);

	double *ur = new double[ncol];
	int *xi = new int[2*ncol]; //Stack
	for (unsigned int r = 0; r < nrow; ++r) {
	    
	    if (_outcome[r] != BGLM_NORMAL) {

		double mu_r = getMean(r);
		double tau_r = getPrecision(r);
		
		//Calculate mean and precision of z[r] conditional
		//on z[s] for s != r
		double zr_mean = 0;
		double Hr = 0; // 
		int top = cs_spsolve(N->L, Pt_x, r, xi, ur, 0, 1);
		for (unsigned int j = top; j < ncol; ++j) {
		    zr_mean  += ur[xi[j]] * w[xi[j]];
		    Hr  += ur[xi[j]] * ur[xi[j]];
		}
		Hr *= tau_r;
		zr_mean -= Hr * (_z[r] - mu_r);
		zr_mean /= (1 - Hr);
		double zr_prec = (1 - Hr) * tau_r;
		
		if (zr_prec <= 0) {
		    throw runtime_error("Invalid precision in Holmes-Held update method.\nThis is a known bug and we are working on it.\nPlease bear with us");
		}

		double yr = schildren[r]->value(_chain)[0];
		double zold = _z[r];
		if (yr == 1) {
		    _z[r] = lnormal(0, rng, mu_r + zr_mean, 1/sqrt(zr_prec));
		}
		else if (yr == 0) {
		    _z[r] = rnormal(0, rng, mu_r + zr_mean, 1/sqrt(zr_prec));
		}
		else {
		    throw logic_error("Invalid child value in HolmesHeld");
		}

		//Add new contribution of row r back to b
		double zdelta = _z[r] - zold;
		for (unsigned int j = top; j < ncol; ++j) {
		    w[xi[j]] += ur[xi[j]] * zdelta * tau_r; 
		}
	    }
	}

	//Free workspace
	delete [] ur;
	//delete [] xr;
	delete [] xi;
	cs_spfree(Pt_x);
    }
    
    void HolmesHeld::update(RNG *rng)
    {
	updateLM(rng);
	
	for (unsigned int r = 0; r < _tau.size(); ++r)
	{
	    if (_outcome[r] == BGLM_LOGIT) {
		double delta = fabs(getValue(r) - getMean(r));
		_tau[r] = 1/sample_lambda(delta, rng);
	    }
	}
    }

    string HolmesHeld::name() const
    {
	return "Holmes-Held";
    }
}
    
