#include <config.h>
#include <sampler/RWMetropolis.h>
//#include <sampler/ParallelDensitySampler.h>
#include <rng/RNG.h>

#include <cmath>
#include <stdexcept>

using std::logic_error;
using std::vector;
using std::log;
using std::exp;
using std::fabs;

/* 
   The value _n controls the reduction in the step size when rescale is
   called. There is no reason to give it an initial value of zero. In
   fact this is a poor choice since the  step size would be immediately
   halved. We start with a value of 10 so the first change in step size
   is 10%.
*/
#define INITIAL_N 10

RWMetropolis::RWMetropolis(vector<StochasticNode*> const &nodes,
			   double step, double prob)
    : Metropolis(nodes),
      _prob(prob), _lstep(log(step)), _p_over_target(false), _n(INITIAL_N),
      _pmean(0), _niter(2)
{
    if (prob < 0 || prob > 1 || step < 0)
	throw logic_error("Invalid initial values in RWMetropolis");
}

RWMetropolis::~RWMetropolis()
{
}

void RWMetropolis::rescale(double p)
{
    _lstep +=  (p - _prob) / _n; 

    if ((p > _prob) != _p_over_target) {
	//   Reduce the step size only when the acceptance probability
	//  crosses the target value. This allows us to adapt quickly
	// to a poor initial choice of scale.
	_p_over_target = !_p_over_target;
	_n++;
    }

    // We keep a weighted mean estimate of the mean acceptance probability
    //  with the weights in favour of more recent iterations
    _pmean += 2 * (p - _pmean) / _niter;
    _niter++;
}

/*
void RWMetropolis::update(RNG *rng)
{
    unsigned int d = value_length();
    double *new_value = new double[d];
    double const *old_value = value();

    double log_p = -_sampler->logFullConditional(_chain);
    double step = exp(_lstep);
    for (unsigned int i = 0; i < d; ++i) {
        new_value[i] = old_value[i] + step * rng->normal();
    }
    propose(new_value, d);
    log_p += _sampler->logFullConditional(_chain);
    double p = exp(log_p);
    accept(rng, p);

    delete [] new_value;
}
*/

bool RWMetropolis::checkAdaptation() const
{
    if (_pmean == 0 || _pmean == 1) {
	return false;
    }
    double logit_target = log(_prob/(1 - _prob));
    double logit_accept = log(_pmean/(1 - _pmean));

    return fabs(logit_target - logit_accept) < 0.5;
}

double RWMetropolis::step() const
{
    return exp(_lstep);
}
