#include <config.h>
#include <sampler/StepAdapter.h>

#include <stdexcept>
#include <algorithm>
#include <cmath>

using std::min;
using std::log;
using std::exp;
using std::logic_error;

/* 
   The value _n controls the reduction in the step size when rescale is
   called. There is no reason to give it an initial value of zero. In
   fact this is a poor choice since the  step size would be immediately
   halved. We start with a value of 10 so the first change in step size
   is 10%.
*/
#define INITIAL_N 10

StepAdapter::StepAdapter(double step, double prob)
    : _prob(prob), _lstep(log(step)), _p_over_target(false), _n(INITIAL_N)
{
    if (prob < 0 || prob > 1 || step < 0)
	throw logic_error("Invalid initial values in StepAdapter");
}

void StepAdapter::rescale(double p)
{
    p = min(p, 1.0);
    _lstep +=  (p - _prob) / _n; 

    if ((p > _prob) != _p_over_target) {
	//   Reduce the step size only when the acceptance probability
	//  crosses the target value. This allows us to adapt quickly
	// to a poor initial choice of scale.
	_p_over_target = !_p_over_target;
	_n++;
    }
}

double StepAdapter::stepSize() const
{
    return exp(_lstep);
}

double StepAdapter::logitDeviation(double p) const
{
    double logit_target = log(_prob/(1 - _prob));
    double logit_p = log(p/(1 - p));

    return logit_target - logit_p;
}
