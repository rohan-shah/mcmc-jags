#include <config.h>

#include "ScaleMixture.h"
#include <graph/StochasticNode.h>
#include <sampler/Updater.h>
#include <rng/TruncatedNormal.h>
#include <rng/RNG.h>

#include <stdexcept>

using std::vector;
using std::string;
using std::logic_error;

static unsigned int nchildren(Updater const *updater)
{
    return updater->stochasticChildren().size();
}

static double LNorm(double mu, double sigma, double left, RNG *rng) {
    return mu + sigma * lnormal((left - mu)/sigma, rng);
}

static double RNorm(double mu, double sigma, double right, RNG *rng) {
    return mu + sigma * rnormal((right - mu)/sigma, rng);
}

namespace glm {

    ScaleMixture::ScaleMixture(Updater const *updater,
		   vector<Updater const *> const &sub_updaters,
		   unsigned int chain)
	: GLMMethod(updater, sub_updaters, chain, true), _z(nchildren(updater))
    {
    }

    double ScaleMixture::getMean(unsigned int i) const
    {
	Node const *child = _updater->stochasticChildren()[i];
	return child->parents()[0]->parents()[0]->value(_chain)[0];
    }
    
    double ScaleMixture::getValue(unsigned int i) const 
    {
	return _z[i];
    }
    
    void ScaleMixture::initAuxiliary(RNG *rng)
    {
	vector<StochasticNode const *> const &schildren = 
	    _updater->stochasticChildren();

	for (unsigned int r = 0; r < _z.size(); ++r) {
	    double yr = schildren[r]->value(_chain)[0];
	    if (yr == 1) {
		_z[r] = LNorm(getMean(r), 1, 0, rng);
	    }
	    else if (yr == 0) {
		_z[r] = RNorm(getMean(r), 1, 0, rng);
	    }
	    else {
		throw logic_error("Invalid child value in ScaleMixture");
	    }
	}
    }
    
    void ScaleMixture::updateAuxiliary(double *w, csn const *N, RNG *rng)
    {
	/* 
	   In the parent GLMMethod class, the posterior precision is
	   represented by the matrix "A"; the posterior mean "mu"
	   solves A %*% mu = b.
	   
	   In this call, "w" solves A %*% w = P %*% b and "N" holds
	   the Cholesky decomposition of P %*% A %*% t(P). 
	*/

	vector<StochasticNode const *> const &schildren = 
	    _updater->stochasticChildren();

	unsigned int nrow = schildren.size();
	unsigned int ncol = _updater->length();

	//Transpose and permute the design matrix
	cs *t_X = cs_transpose(_X, 1);
	cs *Pt_X = cs_permute(t_X, _symbol->pinv, 0, 1);
	cs_spfree(t_X);

	double *ur = new double[ncol];
	int *xi = new int[2*ncol]; //Stack
	for (unsigned int r = 0; r < nrow; ++r) {
	    
	    int top = cs_spsolve(N->L, Pt_X, r, xi, ur, 0, 1);
	    //Subtract contribution of row r from b
	    double mu_r = getMean(r);
	    double delta = _z[r] - mu_r;
	    double tau_r = getPrecision(r);
	    for (unsigned int j = top; j < ncol; ++j) {
		w[xi[j]] -= ur[xi[j]] * delta * tau_r;
	    }
	    
	    //Calculate mean and precision of z[r] conditional
	    //on z[s] for s != r
	    double zr_mean = 0;
	    double Hr = 0; // 
	    for (unsigned int j = top; j < ncol; ++j) {
		zr_mean  += ur[xi[j]] * w[xi[j]];
		Hr  += ur[xi[j]] * ur[xi[j]];
	    }
	    Hr *= tau_r;

	    zr_mean /= (1 - Hr);
	    double zr_prec = (1 - Hr) * tau_r;

	    double yr = schildren[r]->value(_chain)[0];
	    if (yr == 1) {
		_z[r] = LNorm(mu_r + zr_mean, 1/sqrt(zr_prec), 0, rng);
	    }
	    else if (yr == 0) {
		_z[r] = RNorm(mu_r + zr_mean, 1/sqrt(zr_prec), 0, rng);
	    }
	    else {
		throw logic_error("Invalid child value in ScaleMixture");
	    }

	    //Add new contribution of row r back to b
	    delta = _z[r] - mu_r;
	    for (unsigned int j = top; j < ncol; ++j) {
		w[xi[j]] += ur[xi[j]] * delta * tau_r; 
	    }
	}

	//Free workspace
	delete [] ur;
	//delete [] xr;
	delete [] xi;
	cs_spfree(Pt_X);

	updatePrecision(rng);
    }

}
