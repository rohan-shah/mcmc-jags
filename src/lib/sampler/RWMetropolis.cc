#include <config.h>
#include <sampler/RWMetropolis.h>
#include <rng/RNG.h>

#include <cmath>
#include <stdexcept>

using std::logic_error;
using std::vector;

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
    : Metropolis(nodes, graph), _chain(chain), _value(0), 
      _prob(prob), _lscale(log(scale)), _p_over_target(false), _n(INITIAL_N)
{
    if (prob < 0 || prob > 1 || scale < 0 || length == 0)
	throw logic_error("Invalid initial values in RWMetropolis");
    if (length != size()) {
        throw logic_error("Invalid length of initial value in RWMetropolis");
    }
   _value = new double[size()];
}

RWMetropolis::~RWMetropolis()
{
   delete [] _value;
}

void RWMetropolis::rescale(double p, bool accept)
{
    _lscale +=  (p - _prob) / (_n + 1); //We work on a log scale
    bool p_over_target = (p > _prob);
    if (p_over_target != _p_over_target) {
	/* 
	   Reduce the step size only when the acceptance probability
	   crosses the target value. This allows us to adapt quickly
	   to a poor initial choice of scale.
	*/
	_p_over_target = p_over_target;
	_n++;
    }
}

void RWMetropolis::update(RNG *rng)
{
    double scale = exp(_lscale);
    double log_p = -logFullConditional(_chain);
    unsigned int d = size();
    for (unsigned int i = 0; i < d; ++i) {
        _value[i] += scale * rng->uniform();
    }
    propose(_value, d);
    log_p += logFullConditional(_chain);
    accept(rng, exp(log_p));
}
