#include <config.h>
#include <sampler/RWMetropolis.h>
#include <rng/RNG.h>

#include <cmath>
#include <stdexcept>

using std::logic_error;
using std::vector;
using std::log;
using std::exp;

/* 
   The value _n controls the reduction in the step size when rescale is
   called. There is no reason to give it an initial value of zero. In
   fact this is a poor choice since the  step size would be immediately
   halved. We start with a value of 10 so the first change in step size
   is 10%.
*/
#define INITIAL_N 10

RWMetropolis::RWMetropolis(vector<StochasticNode *> const &nodes, 
			   Graph const &graph, unsigned int chain,
                           double const *value, unsigned int length,
                           double scale, double prob)
    : Metropolis(nodes, graph, chain, value, length), 
      _prob(prob), _lscale(log(scale)), _p_over_target(false), _n(INITIAL_N)
{
    if (prob < 0 || prob > 1 || scale < 0 || length == 0)
	throw logic_error("Invalid initial values in RWMetropolis");
}

RWMetropolis::~RWMetropolis()
{
}

void RWMetropolis::rescale(double p)
{
    _lscale +=  (p - _prob) / _n; //We work on a log scale
    if ((p > _prob) != _p_over_target) {
	/* 
	   Reduce the step size only when the acceptance probability
	   crosses the target value. This allows us to adapt quickly
	   to a poor initial choice of scale.
	*/
	_p_over_target = !_p_over_target;
	_n++;
    }
}

void RWMetropolis::update(RNG *rng)
{
    unsigned int d = value_length();
    double *new_value = new double[d];
    double const *old_value = value();

    double log_p = -logFullConditional(chain());
    double scale = exp(_lscale);
    for (unsigned int i = 0; i < d; ++i) {
        new_value[i] = old_value[i] + scale * rng->uniform();
    }
    propose(new_value, d);
    log_p += logFullConditional(chain());
    accept(rng, exp(log_p));

    delete [] new_value;
}
