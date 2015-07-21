#include <config.h>
#include "RESampler.h"
#include <graph/StochasticNode.h>
#include <graph/NodeError.h>
#include <distribution/Distribution.h>
#include <sarray/nainf.h>
#include <rng/RNG.h>
#include <module/ModuleError.h>

#include <algorithm>
#include <cmath>
#include <algorithm>
#include <cfloat>

using std::vector;
using std::copy;

using std::exp;
using std::sqrt;
using std::min;

static vector<StochasticNode*> merg(StochasticNode *variance,
			     vector<StochasticNode*> const &effects)
{
    vector<StochasticNode*> nodes(1 + effects.size());
    nodes[0] = variance;
    vector<StochasticNode*>::iterator p = nodes.begin();
    ++p;
    copy(effects.begin(), effects.end(), p);
    return nodes;
}

namespace jags {
namespace bugs {

RESampler::RESampler(StochasticNode *variance,
		     vector<StochasticNode *> const &effects,
		     Graph const &graph, unsigned int chain,
		     double const *value, unsigned int length)
    : Metropolis(merg(variance,effects), graph, chain, value, length),
      _mode(RE_INIT)
{
    gv->checkFinite(chain); //Check validity of initial values
    support(&_lower, &_upper, 1U, variance, chain);

    for (unsigned int i = 0; i < 3; ++i) {
	_lscale[i] = 0;
	_n[i] = 10;
	_p_over_target[i] = false;
    }
}

RESampler::~RESampler()
{
}

void RESampler::update(RNG* rng)
{
    unsigned int N = value_length();
    double const *old_value = value();
    double *new_value = new double[N];
    copy(old_value, old_value + N, new_value);

    if (_mode == RE_INIT) {
	/* On the first iteration, we initialize the random effects
	   with their prior distribution, and then return */
	for (unsigned int i = 1; i < N; ++i) {
	    double tau = nodes()[i]->parents()[1]->value(chain())[0];
	    new_value[i] = rng->normal() / sqrt(tau);
	}
	propose(new_value, N);
	accept(rng, 1); //Force acceptance
	_mode = RE_LOCATION;
	return; 
    }

    //Calculate mean and variance of random effects
    double mean = 0;
    double var = 0;
    for (unsigned int i = 1; i < N; ++i) {
	mean += (old_value[i] - mean)/i;
	double delta = (old_value[i] - mean);
	var += (delta*delta - var)/i;
    }

    //Step 1: Shuffle random effects

    _mode = RE_SHUFFLE;
    double logdensity0 = logFullConditional(chain());
    
    //Add increment to each random effect
    double new_mean = 0;
    double new_var = 0;
    double scale = exp(_lscale[0]);
    for (unsigned int i = 1; i < N; ++i) {
	new_value[i] += rng->normal() * scale;
	new_mean += (new_value[i] - new_mean)/i;
	double delta = new_value[i] - new_mean;
	new_var += (delta * delta - new_var)/i;
    }
    
    //Fix mean and variance to be the same as the previous value
    double adjust_scale = sqrt(var/new_var);
    for (unsigned int i = 1; i < N; ++i) {
	new_value[i] = mean + (new_value[i] - new_mean) * adjust_scale;
    }
    
    propose(new_value, N);
    double logdensity1 = logFullConditional(chain());
    if (accept(rng, exp(logdensity1 - logdensity0))) {
	logdensity0 = logdensity1;
    }

    //Step 2 modify variance component and the variance of the
    //random effects simultaneously
    _mode = RE_SCALE;
    scale = exp(_lscale[1]);
    new_value[0] += rng->normal() * scale;
    adjust_scale = exp(rng->normal() * scale);
    for (unsigned int i = 1; i < N; ++i) {
	new_value[i] *=  adjust_scale;
    }
    propose(new_value, N);
    logdensity1 = logFullConditional(chain());
    if (accept(rng, exp(logdensity1 - logdensity0))) {
	logdensity0 = logdensity1;
    }

    //Step 3, shift all random effects up or down by the same amount
    _mode = RE_LOCATION;
    double location_shift = rng->normal() * exp(_lscale[2]);
    for (unsigned int i = 1; i < N; ++i) {
	new_value[i] += location_shift;
    }
    logdensity1 = logFullConditional(chain());
    accept(rng, exp(logdensity1 - logdensity0));
}
 
void RESampler::rescale(double p)
{
    p = min(p, 1.0);
    double ptarget = (_mode == RE_SHUFFLE) ? 0.234 : 0.45;

    switch(_mode) {
    case RE_SHUFFLE: case RE_SCALE: case RE_LOCATION:
	_lscale[_mode] += (p - ptarget)/_n[_mode];
	if ((p > ptarget) != _p_over_target[_mode]) {
	    _p_over_target[_mode] = !_p_over_target[_mode];
	    _n[_mode]++;
	}
	break;
    case RE_INIT:
	break;
    }
}


void RESampler::transformValues(const double *v, unsigned int length, 
 				double *nv, unsigned int nlength) 
     const
{
    if (length != value_length() || nlength != length) {
	throwLogicError("Length error in RESampler::transformValues");
    }
    
    bool bb = jags_finite(_lower); //bounded below
    bool ba = jags_finite(_upper); //bounded above
    if (bb && ba) {
	double w = 1.0 / (1 + exp(-v[0]));
	nv[0] = (1 - w) * _lower + w * _upper;
    }
    else if (bb) {
	nv[0] = _lower + exp(v[0]);
    }
    else if (ba) {
	nv[0] = _upper - exp(v[0]);
    }
    else {
	nv[0] = v[0];
    }

    copy(v + 1, v + length, nv + 1);
}

bool RESampler::checkAdaptation() const
{
    return true; //FIXME!!
}

}}
