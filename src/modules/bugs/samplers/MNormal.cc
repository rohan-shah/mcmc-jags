#include <config.h>
#include "MNormal.h"
#include <matrix.h>
#include <DMNorm.h>
#include <graph/StochasticNode.h>
#include <rng/RNG.h>
#include <lapack.h>

#include <cmath>
#include <stdexcept>
#include <algorithm>

#define N_REFRESH 100

using std::vector;
using std::copy;
using std::logic_error;

/* Draws a sample from a multivariate normal distribution with given
   variance matrix.
   
   Code adapted from DMNorm::randomSample, which is parameterized in
   terms of the precision matrix, not the mean
*/

static void mvnormSample(double *x, double const *V, int nrow, RNG *rng)
{
    int N = nrow*nrow;
    double * Vcopy = new double[N];
    copy(V, V + N, Vcopy);
    double * w = new double[nrow];

    int info = 0;
    double worktest;
    int lwork = -1;
    // Workspace query
    F77_DSYEV ("V", "L", &nrow, Vcopy, &nrow, w, &worktest, &lwork, &info);
    // Now get eigenvalues/vectors with optimal work space
    lwork = static_cast<int>(worktest + 0.5);
    double * work = new double[lwork];
    F77_DSYEV ("V", "L", &nrow, Vcopy, &nrow, w, work, &lwork, &info);
    delete [] work;

    /* Generate independent random normal variates, scaled by
       the eigen values. We reuse the array w. */
    for (int i = 0; i < nrow; ++i) {
	x[i] = 0;
	w[i] = sqrt(w[i]) * rng->normal();
    }

    /* Now transform them to dependant variates 
       (On exit from DSYEV, Vcopy contains the eigenvectors)
    */
    for (int i = 0; i < nrow; ++i) {
	for (int j = 0; j < nrow; ++j) {
	    x[i] += Vcopy[i + j * nrow] * w[j];
	    //x[i] += Vcopy[j + i * nrow] * w[j]; FIXME???
	}
    }

    delete [] w;
    delete [] Vcopy;
}

MNormalSampler::MNormalSampler(StochasticNode * node, 
			       Graph const &graph, unsigned int chain, 
			       double const *value, unsigned int N)
    : Metropolis(vector<StochasticNode*>(1,node), graph, chain, value, N),
      _mean(0), _var(0), _prec(0), _n(0), _n_isotonic(0), 
      _sump(0), _lstep(0), _nstep(10), _p_over_target(true)
{
    _mean = new double[N];
    _var = new double[N * N];
    _prec = new double[N * N];
    
    // Give initial mean zero and identity variance matrix 
    for (unsigned int i = 0; i < N; ++i) {
	_mean[i] = 0;
	for (unsigned int j = 0; j < N; ++j) {
	    _prec[i + N * j] = _var[i + N * j] = (i == j) ? 1 : 0;
	}
    }
}

MNormalSampler::~MNormalSampler()
{
    delete [] _mean;
    delete [] _var;
    delete [] _prec;
}

void MNormalSampler::update(RNG *rng)
{
    double const *old_value = value();
    unsigned int N = value_length();
    
    double logdensity = -logFullConditional(chain());
    double step = exp(_lstep);

    double *x = new double[N];
    
    /* DMNorm::randomsample(x, _mean, _prec, N, rng);
    for (unsigned int i = 0; i < N; ++i) {
	x[i] = old_value[i] + (x[i] - _mean[i]) * step;
    }
    */
    mvnormSample(x, _var, N, rng);
    for (unsigned int i = 0; i < N; ++i) {
	x[i] = old_value[i] + x[i] * step;
    }

    propose(x, N);
    logdensity += logFullConditional(chain());
    accept(rng, exp(logdensity));

    delete [] x;
}

void MNormalSampler::rescale(double p, bool accept)
{
    ++_n;
    p = fmin(p, 1.0);

    //Adjust scale of proposal distribution to get optimal acceptance
    //rate using a noisy gradient algorithm
    _lstep += (p - 0.234) / _nstep;
    if ((p > 0.234) != _p_over_target) {
	_p_over_target = !_p_over_target;
	++_nstep;
    }

    if (_n_isotonic == 0) {
	/* 
	   Isotonic random walk. Use the identity matrix (scaled by
	   the _lstep parameter) as the precision of the proposal
	   distribution until the acceptance rate lies in an interval
	   around the optimum.
	*/
	_sump += p;
	if (_n % N_REFRESH == 0) {
	    double meanp = _sump / N_REFRESH;
	    if (meanp >= 0.15 && meanp <= 0.40) {
		_n_isotonic = _n;
		_nstep = 10; //reset the step size as we adapt proposal
	    }
	    else {
		_sump = 0;
	    }
	}
    }
    else {
	/* 
	   Adaptive random walk: The variance of the proposal
	   distribution is adapted to the empirical variance of the
	   posterior distribution. 
	   
	   Note the different denominators for the increment to the mean
	   (_n - _n_isotonic) and variance (_n). The mean is estimated
	   using only the sampled values from iteration _n_isotonic + 1
	   onwards. The variance is shrunk towards the prior (identity
	   matrix) in order to ensure a smooth transition from the isotonic
	   random walk.
	*/

	unsigned int N = length();
	double const *x = value();
	for (unsigned int i = 0; i < N; ++i) {
	    _mean[i] += (x[i] - _mean[i]) / (_n - _n_isotonic);
	}
	for (unsigned int i = 0; i < N; ++i) {
	    for (unsigned int j = 0; j < N; ++j) {
		_var[i + N * j] += ((x[i] - _mean[i]) * (x[j] - _mean[j]) -
				    _var[i + N * j]) / _n;
	    }
	}

	/* 
	   The DMNorm::randomSample function used by update is
	    parameterized in terms of the precision.
	*/
	//inverse (_prec, _var, N, true);
    }
}

void MNormalSampler::transformValues(double const *v, unsigned int length,
				     double *nv, unsigned int nlength) const
{
    if (length != nlength) {
	throw logic_error("Invalid length in MNormalSampler::transformValues");
    }
    copy(v, v + length, nv);
}
