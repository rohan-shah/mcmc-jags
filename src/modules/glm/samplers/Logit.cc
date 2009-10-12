#include <config.h>

#include "Logit.h"
#include <graph/StochasticNode.h>
#include <sampler/Updater.h>
#include <rng/TruncatedNormal.h>
#include <rng/RNG.h>

#include <stdexcept>

using std::vector;
using std::string;
using std::logic_error;

#define PI            3.141592653589793238462643383280 
#define PI_SQUARE       9.86960440108936

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

/*
static double plogit(q)
{
    return 1/(1 + exp(-q));
}

static double qlogit(p)
{
    return log(p) - log(1 - p);
}

static double LLogit(double mu, double sigma, double left, RNG *rng) {
    
    double pl = plogit((left - mu)/sigma);
    double u = pl + rng->uniform() * (1 - pl);
    return mu + sigma * qlogit(u);
}

static double RLogit(double mu, double sigma, double right, RNG *rng) {
    
    double pr = plogit((right - mu)/sigma);
    double u = rng->uniform() * pr;
    return mu + sigma * qlogit(u);
}
*/

static bool r_intvl(double U, double lambda)
{
    double z = 1;
    double x = exp(-lambda/2);
    int j = 0;
    while(true) {
	j++;
	int j2 = (j+1)*(j+1);
	z -= j2 * pow(x, j2 - 1);
	if (z > u)
	    return true;
	j++;
	j2 = (j+1)*(j+1);
	z += j2 * pow(x, j2 - 1);
	if (z < u)
	    return false;
    }
}

static bool l_intvl(double u, double lambda)
{
    double h = log(2)/2 + 2.5*log(PI) - 2.5*log(lambda) - 
	PI_SQUARE/(2 * lambda) + lambda/2;
    double logu = log(u);
    double z = 1;
    double x = exp(-PI_SQUARE/(2*lambda));
    double k = lambda / PI_SQUARE;
    int j = 0;
    while(true) {
	j++;
	z -= k*pow(x, j*j-1);
	if (h + log(z) > logu)
	    return true;
	j++;
	int j2 = (j+1)*(j+1);
	z += j2*pow(x,j2-1);
	if (h + log(z) < logu)
	    return false;
    }
}

static double sample_lambda(double delta, RNG *rng)
{
    double y = rng()->normal();
    y = y * y;
    y <- 1 + (y - sqrt(y*(4*delta + y))/(2*delta));
    double u = rng()->uniform();
    double lambda = (u <= 1/(1+y)) ? delta/Y : delta*Y;
    u = rng->uniform();
    bool ok = (lambda > 4.0/3.0) ? r_intvl(u, lambda) : l_intvl(u, lambda);
    if (ok)
	return lambda;
}
	
namespace glm {

    Logit::Logit(Updater const *updater,
		   vector<Updater const *> const &sub_updaters,
		   unsigned int chain)
	: GLMMethod(updater, sub_updaters, chain, true), 
	  _z(nchildren(updater)), _tau(nchildren(updater))
    {
    }

    double Logit::getMean(unsigned int i) const
    {
	Node const *child = _updater->stochasticChildren()[i];
	return child->parents()[0]->parents()[0]->value(_chain)[0];
    }
    
    double 
    Logit::getPrecision(unsigned int i) const 
    {
	return _tau[i];
    }

    double Logit::getValue(unsigned int i) const 
    {
	return _z[i];
    }
    
    string Logit::name() const
    {
	return "Logit";
    }
    
    void Logit::initAuxiliary(RNG *rng)
    {
	vector<StochasticNode const *> const &schildren = 
	    _updater->stochasticChildren();

	for (unsigned int r = 0; r < _z.size(); ++r) {
	    double yr = schildren[r]->value(_chain)[0];
	    if (yr == 1) {
		_z[r] = LNormal(getMean(r), 1, 0, rng);
	    }
	    else if (yr == 0) {
		_z[r] = RNormal(getMean(r), 1, 0, rng);
	    }
	    else {
		throw logic_error("Invalid child value in Logit");
	    }
	    _tau[r] = 1;
	}
    }

    //FIXME: Less cut-and-paste: more inheritance
    void Logit::updateAuxiliary(double *w, csn const *N, RNG *rng)
    {
	//Holmes-Held algorithm
	
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
	    _tau[r] = 1/sample_lambda(delta);
	    for (unsigned int j = top; j < ncol; ++j) {
		w[xi[j]] -= ur[xi[j]] * delta * _tau[r];
	    }
	    
	    //Calculate mean and precision of z[r] conditional
	    //on z[s] for s != r
	    double zr_mean = 0;
	    double Hr = 0; // 
	    for (unsigned int j = top; j < ncol; ++j) {
		zr_mean  += ur[xi[j]] * w[xi[j]];
		Hr  += ur[xi[j]] * ur[xi[j]];
	    }
	    Hr *= _tau[r];

	    zr_mean /= (1 - Hr);
	    double zr_prec = (1 - Hr) * _tau[r];

	    double yr = schildren[r]->value(_chain)[0];
	    if (yr == 1) {
		_z[r] = LNorm(mu_r + zr_mean, 1/sqrt(zr_prec), 0, rng);
	    }
	    else if (yr == 0) {
		_z[r] = RNorm(mu_r + zr_mean, 1/sqrt(zr_prec), 0, rng);
	    }
	    else {
		throw logic_error("Invalid child value in Logit");
	    }

	    //Add new contribution of row r back to b
	    delta = _z[r] - mu_r;
	    for (unsigned int j = top; j < ncol; ++j) {
		w[xi[j]] += ur[xi[j]] * delta * _lambda[r]; 
	    }
	}

	//Free workspace
	delete [] ur;
	//delete [] xr;
	delete [] xi;
	cs_spfree(Pt_X);
    }

}
