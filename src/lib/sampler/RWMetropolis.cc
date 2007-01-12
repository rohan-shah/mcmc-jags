#include <config.h>
#include <sampler/RWMetropolis.h>

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
    : Metropolis(nodes, graph, chain, value, length), 
      _prob(prob), _lscale(log(scale)), _p_over_target(false), _n(INITIAL_N)
{
    if (prob < 0 || prob > 1 || scale < 0)
	throw logic_error("Invalid initial values in RWMetropolis");
}

RWMetropolis::~RWMetropolis()
{
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

double RWMetropolis::scale() const
{
    return exp(_lscale);
}
