#include <config.h>
#include "MixSampler.h"
#include <graph/Graph.h>
#include <graph/StochasticNode.h>
#include <graph/GraphMarks.h>
#include <distribution/Distribution.h>
#include <rng/RNG.h>
#include <sarray/util.h>
#include <sarray/nainf.h>

#include <cmath>
#include <algorithm>
#include <stdexcept>

using std::vector;
using std::copy;
using std::logic_error;
using std::invalid_argument;

#define INIT_N 10

static void read_bounds(vector<StochasticNode*> const &snodes, 
			unsigned int chain,
			double *lower, double *upper, unsigned int length)
{
    double *lp = lower;
    double *up = upper;
    unsigned int node_length = 0;
    for (unsigned int j = 0; j < snodes.size(); ++j) {
	unsigned int length_j = snodes[j]->length();
	node_length += length_j;
	if (node_length > length) {
	    throw logic_error("Invalid length in read_bounds (MixSampler)");
	}
	else {
	    snodes[j]->distribution()->support(lp, up, length_j, 
					       snodes[j]->parameters(chain), 
					       snodes[j]->parameterDims());
	    lp += length_j;
	    up += length_j;
	}
    }
}

MixSampler::MixSampler(vector<StochasticNode *> const &snodes, 
		       Graph const &graph, unsigned int chain,
                       double const *value, unsigned int length, 
		       unsigned int nlevels, double min_power,
		       double target_prob)
    : Metropolis(snodes, graph, chain, value, length), 
      _nlevels(nlevels), _nstep(2*nlevels),
      _log_min_power(log(min_power)), 
      _target_prob(target_prob),
      _lower(0), _upper(0),
      _prob(_nstep), 
      _n(_nstep), 
      _lscale(_nstep), 
      _p_over_target(_nstep),
      _global_lscale(-10),
      _global_p_over_target(false),
      _global_n(INIT_N),
      _temper(false)
{
    if (!canSample(snodes, graph)) {
	throw invalid_argument("Can't construct Mixture sampler");
    }
    if (min_power > 1 || min_power < 0) {
	throw invalid_argument("Invalid min_power in Mixture sampler");
    }

    for (unsigned int j = 0; j < _nstep; ++j) {
	_n[j] = INIT_N;
	_lscale[j] = -10;
	_p_over_target[j] = false;
    }

    _lower = new double[value_length()];
    _upper = new double[value_length()];
    read_bounds(snodes, chain, _lower, _upper, value_length());
}

MixSampler::~MixSampler()
{
    delete [] _lower;
    delete [] _upper;
}

void MixSampler::transformValues(double const *v, unsigned int length,
				 double *nv, unsigned int nlength) const
{
    if (length != value_length() || nlength != length) {
	throw logic_error("Length error in MixSampler::transformValues");
    }

    for (unsigned int i = 0; i < length; ++i) {
	bool bb = jags_finite(_lower[i]); //bounded below
	bool ba = jags_finite(_upper[i]); //bounded above
	if (bb && ba) {
	    double w = 1.0 / (1 - exp(-v[i]));
	    nv[i] = w * _lower[i] + (1 - w) * _upper[i];
	}
	else if (bb) {
	    nv[i] = _lower[i] + exp(v[i]);
	}
	else if (ba) {
	    nv[i] = _upper[i] - exp(v[i]);
	}
	else {
	    nv[i] = v[i];
	}
    }
}

void MixSampler::readValues(vector<StochasticNode*> const &snodes,
			    unsigned int chain,
			    double *value, unsigned int length)
{
    unsigned int value_length = 0;
    for (unsigned int i = 0; i < snodes.size(); ++i) {
         value_length += df(snodes[i]);
    }
    if (length != value_length) {
	throw logic_error("Length error in MixSampler::readValues");
    }

    //Get the lower and upper bounds
    double *lower = new double[length];
    double *upper = new double[length];
    read_bounds(snodes, chain, lower, upper, length);

    //Copy values from the nodes ...
    double *vp = value;
    for (unsigned int j = 0; j < snodes.size(); ++j) {
	double const *value_j = snodes[j]->value(chain);
	unsigned int length_j = snodes[j]->length();
	copy(value_j, value_j + length_j, vp);
	vp += length_j;
    }
    
    // ... transform them, if necessary
    for (unsigned int i = 0; i < length; ++i) {
	bool bb = jags_finite(lower[i]); //bounded below
	bool ba = jags_finite(upper[i]); //bounded above
	if (bb && ba) {
	    value[i] = log(value[i] - lower[i]) - log(upper[i] - value[i]);
	}
	else if (bb) {
	    value[i] = log(value[i] - lower[i]); 
	}
	else if (ba) {
	    value[i] = log(upper[i] - value[i]);
	}
    }
}

bool MixSampler::canSample(vector<StochasticNode *> const &snodes,
                           Graph const &graph)
{
    for (unsigned int i = 0; i < snodes.size(); ++i) {
	if (!graph.contains(snodes[i])) {
	    return false;
	}
	if (snodes[i]->isDiscreteValued()) {
	    return false;
	}
	/* Check that the bounds (if any) of each node are fixed 
	   We rely on parameter transformation for bounded nodes which
	   must remain the same between iterations to guarantee
	   stationarity of the chain
	*/
	vector<bool> fixmask(snodes[i]->parents().size());
	for (unsigned int j = 0; j < snodes[i]->parents().size(); ++j) {
	    fixmask[j] = snodes[i]->parents()[j]->isObserved();
	}
	if (!snodes[i]->distribution()->isSupportFixed(fixmask)) {
	    return false;
	}
	//Check that all nodes are of full rank
	for (unsigned int i = 0; i < snodes.size(); ++i) {
	    if (snodes[i]->length() != df(snodes[i]))
		return false;
	}
    }
    return true;
}

void MixSampler::update(RNG *rng)
{
    unsigned int length = value_length();
    double *proposal = new double[length];
    double *last_proposal = new double[length];
    copy(value(), value() + length, proposal);
    copy(value(), value() + length, last_proposal);

    unsigned int ch = chain();

    //We first do a non-tempered update
    _temper = false;

    double lprob = -logFullConditional(ch);
    double scale = exp(_global_lscale);
    for (unsigned int j = 0;j < length; ++j) {
           proposal[j] = last_proposal[j] + rng->normal() * scale;
    }
    propose(proposal, length);
    lprob += logFullConditional(ch);
    accept(rng, exp(lprob));

    //Now do a tempered update
    _temper = true;

    double lprior0 = logPrior(ch);
    double llik0 = logLikelihood(ch);
    
    double log_global_prob = -lprior0 - llik0;
    for (unsigned int t = 0; t < _nstep; ++t) {

	// Generate new proposal
	double scale = exp(_lscale[t]);
	for (unsigned int j = 0; j < length; ++j) {
	    proposal[j] = last_proposal[j] + rng->normal() * scale;
	}
	propose(proposal, length);

	// Calculate new prior and likelihood
	double lprior1 = logPrior(ch);
	double llik1 = logLikelihood(ch);

	// Calculate power level for tempering
	unsigned int level = (t < _nlevels) ? t : _nstep - 1 - t;
	double pwr = exp(_log_min_power * level / _nlevels);

	// Calculate acceptance probability for new proposal
	double lprob = (lprior1 - lprior0) + pwr * (llik1 - llik0);
	_prob[t] = exp(lprob);
	if (rng->uniform() <= _prob[t]) {
	    //Accept modified proposal
	    copy(proposal, proposal + length, last_proposal);
	    log_global_prob -= lprob;
	    lprior0 = lprior1;
	    llik0 = llik1;
	}
        else if (t == _nstep - 1) {
            //On the last iteration, we must reset the value of the sampler
            //to the last good proposal if the current one fails.
	    propose(last_proposal, length); 
	}
    }
    log_global_prob += lprior0 + llik0;

    delete [] proposal;
    delete [] last_proposal;

    accept(rng, exp(log_global_prob));
}

void MixSampler::rescale(double prob, bool accept)
{
    if (!_temper) {
        double pdiff = fmin(prob, 1.0) - _target_prob;
        _global_lscale += pdiff / _global_n;
        if ((pdiff >= 0) != _global_p_over_target) {
            _global_p_over_target = !_global_p_over_target;
            _global_n++;
        }
    }
    else {
       /* Each step up and down has its own scale */
       for (unsigned int i = 0; i < _nstep; ++i) {
   
	   double pdiff = fmin(_prob[i], 1.0) - _target_prob;
           _lscale[i] +=  pdiff / _n[i]; 
   
	   if ((pdiff >= 0) != _p_over_target[i]) {
	       _p_over_target[i] = !_p_over_target[i];
	       _n[i]++;
	   }
       }
    }
}
