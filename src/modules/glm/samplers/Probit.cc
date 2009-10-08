#include <config.h>

#include "Probit.h"
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
    return mu + sigma * rnormal((mu - right)/sigma, rng);
}

namespace glm {

    Probit::Probit(Updater const *updater,
		   vector<Updater const *> const &sub_updaters,
		   unsigned int chain)
	: GLMMethod(updater, sub_updaters, chain, true), _z(nchildren(updater))
    {
    }

    double Probit::getMean(unsigned int i) const
    {
	Node const *child = _updater->stochasticChildren()[i];
	return child->parents()[0]->parents()[0]->value(_chain)[0];
    }
    
    double 
    Probit::getPrecision(unsigned int i) const 
    {
	return 1;
    }

    double Probit::getValue(unsigned int i) const 
    {
	return _z[i];
    }
    
    string Probit::name() const
    {
	return "Probit";
    }
    
    void Probit::initAuxiliary(RNG *rng)
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
		throw logic_error("Invalid child value in Probit");
	    }
	}
	
    }
    
    void Probit::updateAuxiliary(double *b, csn const *N, RNG *rng)
    {
	vector<StochasticNode const *> const &schildren = 
	    _updater->stochasticChildren();

	unsigned int nrow = schildren.size();
	unsigned int ncol = _updater->length();

	//Create transpose of design matrix for easier access to rows
	cs *t_X = cs_transpose(_X, 1);
	int *Tp = t_X->p;
	int *Ti = t_X->i;
	double *Tx = t_X->x;

	double *xr = new double[ncol]; // Row r of design matrix
	double *S = new double[ncol];  // V %*% xr
	double *w = new double[ncol];  // work space
	for (unsigned int r = 0; r < nrow; ++r) {

	    //Copy row i of design matrix to array xr;
	    for (unsigned int i = 0; i < ncol; ++i) {
		xr[i] = 0;
	    }
	    for (int c = Tp[r]; c < Tp[r+1]; ++c) {
		xr[Ti[c]] = Tx[c];
	    }
	
	    //Subtract contribution of row r to b
	    double mu_r = getMean(r);
	    for (unsigned int i = 0; i < ncol; ++i) {
		b[i] -= (_z[r] - mu_r) * xr[i];
	    }

	    //Calculate mean and variance of z[r] conditional
	    //on z[s] for s != r

	    cs_ipvec(_symbol->pinv, xr, w, ncol);
	    cs_lsolve(N->L, w);
	    cs_ltsolve(N->L, w);
	    cs_pvec(_symbol->pinv, w, S, ncol);
	    
	    double hr = 0; // Hat[r,r]
	    double zr_mean = 0;
	    for (unsigned int i = 0; i < ncol; ++i) {
		hr += S[i] * xr[i];
		zr_mean +=  S[i] * b[i];
	    }
	    zr_mean /= (1 - hr);
	    double zr_var = 1/(1 - hr);

	    double yr = schildren[r]->value(_chain)[0];
	    if (yr == 1) {
		_z[r] = LNorm(mu_r + zr_mean, sqrt(zr_var), 0, rng);
	    }
	    else if (yr == 0) {
		_z[r] = RNorm(mu_r + zr_mean, sqrt(zr_var), 0, rng);
	    }
	    else {
		throw logic_error("Invalid child value in Probit");
	    }

	    //Add new contribution of row r back to b
	    for (unsigned int i = 0; i < ncol; ++i) {
		b[i] += (_z[r] - mu_r) * xr[i];
	    }
	}

	//Free workspace
	delete [] w;
	delete [] S;
	delete [] xr;
	cs_spfree(t_X);

    }
}
