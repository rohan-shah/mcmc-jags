#include <config.h>
#include <sampler/RWMetropolis.h>
#include <rng/RNG.h>

#include <cmath>

using std::vector;
using std::log;
using std::exp;
using std::fabs;

RWMetropolis::RWMetropolis(vector<double> const &value,
			   double step, double prob)
    : Metropolis(value), _step_adapter(step, prob), _pmean(0), _niter(2)
{
}

RWMetropolis::~RWMetropolis()
{
}

void RWMetropolis::rescale(double p)
{
    _step_adapter.rescale(p);

    // We keep a weighted mean estimate of the mean acceptance probability
    //  with the weights in favour of more recent iterations
    _pmean += 2 * (p - _pmean) / _niter;
    _niter++;
}

void RWMetropolis::update(RNG *rng)
{
    vector<double> value(length());
    getValue(value);

    double log_p = -logDensity() - logJacobian(value);
    step(value, _step_adapter.stepSize(), rng);
    setValue(value);
    log_p += logDensity() + logJacobian(value);
    accept(rng, exp(log_p));
}

bool RWMetropolis::checkAdaptation() const
{
    if (_pmean == 0 || _pmean == 1) {
	return false;
    }

    return fabs(_step_adapter.logitDeviation(_pmean)) < 0.5;
}

void RWMetropolis::step(vector<double> &value, double s, RNG *rng) const
{
    for (unsigned int i = 0; i < value.size(); ++i) {
	value[i] += rng->normal() * s;
    }
}

double RWMetropolis::logJacobian(vector<double> const &value) const
{
    return 0;
}
