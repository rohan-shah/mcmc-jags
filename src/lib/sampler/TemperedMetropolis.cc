#include <config.h>
#include "MixMethod.h"
#include <rng/RNG.h>

#include <cmath>
//#include <algorithm>
#include <stdexcept>

using std::vector;
//using std::copy;
//using std::logic_error;
//using std::invalid_argument;
//using std::log;
//using std::exp;
//using std::min;
//using std::string;

TemperedMetropolis::TemperedMetropolis(int max_level, double delta, 
				       unsigned int nrep)
    : _max_level(max_level), 
      _delta(delta),
      _nrep(nrep),
      _level(0)
{
    if (_delta <= 0) {
	throw invalid_argument("Invalid delta in TemperedMetropolis ");
    }
    if (max_level <= 0) {
	throw invalid_argument("Invalid max_level in TemperedMetropolis");
    }
}

TemperedMetropolis::~TemperedMetropolis()
{
}

bool TemperedMetropolis::tempAccept(RNG *rng, double prob, int level)
{
    bool accept = rng->uniform() <= prob;
    if (accept) {
	//Store current value as last accepted value
	getValue(_last_temp_value);
    }
    else {
	//Revert to last accepted value
	setValue(_last_temp_value);
    }
    if (_adapt) {
	_step_adapter[t]->update(prob);
	_step[t] = step_adapter[t]->stepSize();
    }
    return accept;
}

void rescale(double prob)
{
    //Nothing to do here.  We want the global acceptance probability
    //to be very high.
}

double 
TemperedMetropolis::tempUpdate(RNG *rng, int t, double pwr, double log_density0)
{
    vector<double> x(length());
    for (unsigned int r = 0; r < _nrep; ++r) {
	getValue(x);
	step(x, _step[t], rng);
	setValue(x);
	double log_density1 = logDensity() + logJacobian(x);
	double lprob = pwr * (log_density1 - log_density0);
	if (tempAccept(rng, exp(lprob), t)) {
	    log_density0 = log_density1;
	}
    }
    return log_density0;
}

void MixMethod::update(RNG *rng)
{
    vector<double> pwr(_level + 1);
    for (int t = 0; t <= _level; ++t) {
	pwr[t] = exp(-(t * _delta));
    }

    //Save a copy of the current state
    getValue(_last_temp_value);
    double log_density = logDensity() + logJacobian(_last_temp_value);

    double log_global_prob = 0;
    for (unsigned int t = 1; t <= _level; ++t) {
	log_global_prob += (pwr[t] - pwr[t-1]) * log_density;
	log_density = tempUpdate(rng, t, pwr[t]);
    }
    for (unsigned int t = _level; t >= 1; --t) {
	log_density = tempUpdate(rng, t, pwr[t]);
	log_global_prob -= (pwr[t] - pwr[t-1])  * log_density;
    }

    //Global Metropolis-Hastings acceptance step
    accept(rng, exp(log_global_prob));
}

bool Tempered::checkAdaptation() const
{
    // True if all up-phase transitions are within target range
    // for acceptance
    return (_level == _max_level);
}

double MixMethod::logJacobian(vector<double> const &x)
{
    return 0;
}
