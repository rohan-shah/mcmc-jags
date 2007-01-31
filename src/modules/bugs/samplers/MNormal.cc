#include <config.h>
#include "MNormal.h"
#include <matrix.h>
#include <DMNorm.h>
#include <graph/StochasticNode.h>

#include <cmath>
#include <stdexcept>
#include <algorithm>

#define N_BURNIN 100

using std::vector;
using std::copy;
using std::logic_error;

MNormalSampler::MNormalSampler(StochasticNode * node, 
			       Graph const &graph, unsigned int chain, 
			       double const *value, unsigned int length)
    : Metropolis(vector<StochasticNode*>(1, node), graph, chain, value, length), _mean(0), _var(0),
      _n(0), _sump(0), _lstep(0)
{
    _mean = new double[length];
    _var = new double[length * length];
    for (unsigned int i = 0; i < length; ++i) {
	_mean[i] = 0;
	for (unsigned int j = 0; j < length; ++j) {
	    _var[i + length * j] = (i == j) ? 1 : 0;
	}
    }
}

void MNormalSampler::update(RNG *rng)
{
    double const *old_value = value();
    unsigned int N = value_length();
    double *x = new double[N];
    
    //randomsample is parameterized in terms of precision matrix
    //so we have to calculate this first

    double *prec = new double[N*N];
    inverse (prec, _var, N, true);
    DMNorm::randomsample(x, _mean, prec, N, rng);
    delete [] prec;

    double logdensity0 = logFullConditional(chain());
    double step = exp(_lstep);
    for (unsigned int i = 0; i < N; ++i) {
	x[i] = old_value[i] + (x[i] - _mean[i]) * step;
    }
    propose(x, N);
    double logdensity1 = logFullConditional(chain());
    accept(rng, exp(logdensity1 - logdensity0));

    delete [] x;
}

void MNormalSampler::rescale(double p, bool accept)
{
    const double p_min = 0.20;
    const double p_max = 0.45;
    
    ++_n;

    p = fmin(p, 1.0);

    _sump += p;
    if (_n % N_BURNIN == 0) {
	
	double p_mean = _sump / N_BURNIN;
	if (p_mean < p_min) {
	    _lstep -= log(1.5);
	}
	else if (p_mean > p_max) {
	    _lstep += log(1.5);
	}
	_sump = 0;
    }


    if (_n > N_BURNIN) {
	unsigned int N = length();
	double const *v = value();
	for (unsigned int i = 0; i < N; ++i) {
	    _mean[i] += (v[i] - _mean[i])/_n;
	}
	for (unsigned int i = 0; i < N; ++i) {
	    for (unsigned int j = 0; j < N; ++j) {
		_var[i + N * j] += ((v[i] - _mean[i]) * (v[j] - _mean[j]) -
				    _var[i + N * j]) / _n;
	    }
	}
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
