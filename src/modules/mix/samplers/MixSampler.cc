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

static unsigned int addLength(vector<StochasticNode*> const &snodes)
{
    unsigned int length = 0;
    for (unsigned int i = 0; i < snodes.size(); ++i) {
	length += snodes[i]->length();
    }
    return length;
}


MixSampler::MixSampler(vector<StochasticNode *> const &snodes, 
		       Graph const &graph, unsigned int chain,
		       unsigned int nlevels, double min_power,
		       double target_prob)
    : Metropolis(snodes, graph), _chain(chain), 
      _nlevels(nlevels), _nstep(2*nlevels+1),
      _log_min_power(log(min_power)), _scale(2 * nlevels + 1), 
      _length(addLength(snodes)),  _target_prob(target_prob),
      _value(0), _lower(0), _upper(0),
      _prob(_nstep), 
      _n(_nstep), 
      _lscale(_nstep), 
      _p_over_target(_nstep)
{
    if (!canSample(snodes, graph)) {
	throw invalid_argument("Can't construct Mixture sampler");
    }
    if (min_power > 1 || min_power < 0) {
	throw invalid_argument("Invalid min_power in Mixture sampler");
    }

    for (unsigned int j = 0; j < _nstep; ++j) {
	_p_over_target[j] = false;
	_lscale[j] = 0;
	_n[j] = INIT_N;
    }

    //Set up lower and upper bounds
    double *lower = _lower;
    double *upper = _upper;
    for (unsigned int j = 0; j < snodes.size(); ++j) {
	unsigned int length_j = snodes[j]->length();
	snodes[j]->distribution()->support(lower, upper, length_j, 
					   snodes[j]->parameters(_chain), 
					   snodes[j]->parameterDims());
	lower += length_j;
	upper += length_j;
    }
    //Read current value of nodes (with transformation) into _value
    getValue(_value, _length);
}

void MixSampler::propose(double const *value, unsigned int length)
{
    if (length != _length) {
	throw logic_error("Length error in MixSampler::propose");
    }

    double * nodes_value = new double[_length];
    for (unsigned int i = 0; i < _length; ++i) {
	bool bb = jags_finite(_lower[i]);
	bool ba = jags_finite(_upper[i]);
	if (bb & ba) {
	    double w = 1.0 / (1 - exp(-_value[i]));
	    nodes_value[i] = (1 - w) * _lower[i] + w * _upper[i];
	}
	else if (bb) {
	    nodes_value[i] = _lower[i] + exp(_value[i]);
	}
	else if (bb) {
	    nodes_value[i] = _upper[i] - exp(_value[i]);
	}
	else {
	    nodes_value[i] = _value[i];
	}
    }
    setValue(nodes_value, length, chain());
    delete [] nodes_value;
}

void MixSampler::getValue(double *value, unsigned int length)
{
    if (length != _length) {
	throw logic_error("Length error in MixSampler::getValue");
    }
    
    vector<StochasticNode*> const &snodes = nodes();
    unsigned int k = 0;
    for (unsigned int j = 0; j < snodes.size(); ++j) {
	double const *v = snodes[j]->value(_chain);
	unsigned int len = snodes[j]->length();
	for (unsigned int i = 0; i < len; ++i, ++k) {
	    bool bb = jags_finite(v[i]);
	    bool ba = jags_finite(v[i]);
	    if (bb & ba) {
		value[k] = log(v[i] - _lower[k]) - log(_upper[k] - v[i]);
	    }
	    else if (bb) {
		value[k] = log(v[i] - _lower[k]); 
	    }
	    else if (ba) {
		value[k] = log(_upper[k] - v[i]);
	    }
	    else {
		value[k] = v[i];
	    }
	}
    }
}

bool MixSampler::canSample(vector<StochasticNode *> snodes, Graph const &graph)
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
    double *last_value = new double[_length];
    copy(last_value, last_value + _length, _value);

    /* 
       Set up vector of power steps up and down, with uniform
       geometric progression between maximum and minimum
    */
    vector<double> pwr(_nstep);
    for (unsigned int i = 0; i < _nlevels - 1; ++i) {
	pwr[_nstep - 1 - i] = pwr[i] = exp(_log_min_power * (i+1) / _nlevels);
    }

    unsigned int ch = chain();
    double log_global_prob = -logLikelihood(ch);
    for (unsigned int t = 0; t < _nstep; ++t) {
	/* Now calculate the log density under the tempered
	   distribution.  Following Celeux et al (2000) we temper only
	   the likelihood part of the log density */
	double ldensity0 = logPrior(ch) + pwr[t] * logLikelihood(ch);
	double scale = exp(_lscale[t]);
	for (unsigned int j = 0; j < _length; ++j) {
	    _value[j] = last_value[j] + rng->normal() * scale;
	}
	propose(_value, _length);
	double ldensity1 = logPrior(ch) + pwr[t] * logLikelihood(ch);
	double lprob = ldensity1 - ldensity0;
	log_global_prob -= lprob;
	_prob[t] = (lprob > 0) ? 1.0 : exp(lprob);
	if (rng->uniform() <= _prob[t]) {
	    //Accept modified proposal
	    copy(_value, _value + _length, last_value);
	}
	else {
	    //FIXME: we only have to use propose on the last step
	    propose(last_value, _length); //Revert to previous proposal
	}
    }
    log_global_prob += logLikelihood(_chain);

    delete [] last_value;

    accept(rng, exp(log_global_prob));
}

void MixSampler::rescale(double prob, bool accept)
{
    /* Each step up and down has its own scale */
    for (unsigned int i = 0; i < _nstep; ++i) {
	double pdiff = _prob[i] - _target_prob;
        _lscale[i] +=  pdiff / (_n[i] + 1); 
	bool p_over_target = (pdiff >= 0);
	if (p_over_target != _p_over_target[i]) {
	    _p_over_target[i] = p_over_target;
	    _n[i]++;
	}
    }
}

double const *MixSampler::value() const
{
    return _value;
}
