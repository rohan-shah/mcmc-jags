#include <config.h>
#include "MNormal.h"

#include <matrix.h>
#include <DMNorm.h>
#include <lapack.h>

#include <graph/StochasticNode.h>
#include <rng/RNG.h>

#include <cmath>
#include <stdexcept>
#include <algorithm>

#define N_REFRESH 100

using std::vector;
using std::copy;
using std::logic_error;

MNormSampler::MNormSampler(StochasticNode * node, 
			       Graph const &graph, unsigned int chain, 
			       double const *value, unsigned int N)
    : Metropolis(vector<StochasticNode*>(1,node), graph, chain, value, N),
      _mean(0), _var(0), _prec(0), _n(0), _n_isotonic(0), 
      _sump(0), _meanp(0), _lstep(0), _nstep(10), _p_over_target(true)
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

MNormSampler::~MNormSampler()
{
    delete [] _mean;
    delete [] _var;
    delete [] _prec;
}

void MNormSampler::update(RNG *rng)
{
    double const *old_value = value();
    unsigned int N = value_length();
    
    double logdensity = -logFullConditional(chain());
    double step = exp(_lstep);

    double *x = new double[N];
    
    DMNorm::randomsample(x, 0, _var, false, N, rng);
    for (unsigned int i = 0; i < N; ++i) {
	x[i] = old_value[i] + x[i] * step;
    }

    propose(x, N);
    logdensity += logFullConditional(chain());
    accept(rng, exp(logdensity));

    delete [] x;
}

void MNormSampler::rescale(double p, bool accept)
{
    ++_n;
    p = fmin(p, 1.0);
    _sump += p;

    if (_n % N_REFRESH == 0) {
	//Calculate the running mean acceptance rate 
	_meanp = _sump / N_REFRESH;    
	_sump = 0;
    }

    if (_n_isotonic == 0) {
	//Adjust scale of proposal distribution to get optimal acceptance
	//rate using a noisy gradient algorithm
	_lstep += (p - 0.234) / _nstep;
	if ((p > 0.234) != _p_over_target) {
	    _p_over_target = !_p_over_target;
	    ++_nstep;
	}
	/* 
	   Isotonic random walk. Use the identity matrix (scaled by
	   the _lstep parameter) as the precision of the proposal
	   distribution until the acceptance rate lies in an interval
	   around the optimum.
	*/
	if (_n % N_REFRESH == 0 && _meanp >= 0.15 && _meanp <= 0.40) {
	    _n_isotonic = _n;
	    _nstep = 100; //reset the step size as we adapt proposal
	}
    }
    else {
	_lstep += (p - 0.234) / sqrt(_nstep); //testing
        _nstep++;
/*
	if ((p > 0.234) != _p_over_target) {
	    _p_over_target = !_p_over_target;
	    ++_nstep;
	}
*/
	/* 
	   Adaptive random walk: The variance of the proposal
	   distribution is adapted to the empirical variance of the
	   posterior distribution.
	   
	   We use weighted moment estimators for the mean and variance
	   so that more recent iterations get greter weight. This is
	   important because the chain has not converged: the effect
	   of an initial transient must be minimized.

	   The weights are proportional to (_n - _n_isotonic) for the
	   mean, and the iterative formula is exact.  For the
	   variance, the weights are proportional to _n, and the
	   formula is asymptotically correct.

	   For small values of (_n - _n_isotonic), the variance
	   estimator is shrunk towards the prior (identity matrix) in
	   order to ensure a smooth transition from the isotonic
	   random walk.
	*/

	unsigned int N = length();
	double const *x = value();
	for (unsigned int i = 0; i < N; ++i) {
	    _mean[i] += 2 * (x[i] - _mean[i]) / (_n - _n_isotonic + 1);
	}
	for (unsigned int i = 0; i < N; ++i) {
	    for (unsigned int j = 0; j < N; ++j) {
		_var[i + N * j] += 2 * ((x[i] - _mean[i]) * (x[j] - _mean[j]) -
					_var[i + N * j]) / _n;
	    }
	}
    }
}

void MNormSampler::transformValues(double const *v, unsigned int length,
				     double *nv, unsigned int nlength) const
{
    if (length != nlength) {
	throw logic_error("Invalid length in MNormSampler::transformValues");
    }
    copy(v, v + length, nv);
}

bool MNormSampler::checkAdaptation() const
{
    return (_meanp >= 0.15) && (_meanp <= 0.35);
}
