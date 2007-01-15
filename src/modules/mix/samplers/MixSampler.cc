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

MixSampler::MixSampler(vector<StochasticNode *> const &snodes, 
		       Graph const &graph, unsigned int chain,
		       unsigned int nlevels, double min_power,
		       double target_prob)
    : Metropolis(snodes, graph), _chain(chain), 
      _nlevels(nlevels), _nstep(2*nlevels-1),
      _log_min_power(log(min_power)), _target_prob(target_prob),
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
	_n[j] = INIT_N;
	_lscale[j] = -10;
	_p_over_target[j] = false;
    }

    _lower = new double[value_length()];
    _upper = new double[value_length()];
    //Set up lower and upper bounds
    double *lp = _lower;
    double *up = _upper;
    for (unsigned int j = 0; j < snodes.size(); ++j) {
	unsigned int length_j = snodes[j]->length();
	snodes[j]->distribution()->support(lp, up, length_j, 
					   snodes[j]->parameters(_chain), 
					   snodes[j]->parameterDims());
	lp += length_j;
	up += length_j;
    }

    //Read current value of nodes (with transformation) into _value
    _value = new double[value_length()];
    getValue(_value, value_length());
}

MixSampler::~MixSampler()
{
    delete [] _lower;
    delete [] _upper;
    delete [] _value;
}

void MixSampler::propose(double const *v, unsigned int length)
{
    if (length != value_length()) {
	throw logic_error("Length error in MixSampler::propose");
    }

    double * nodes_value = new double[length];
    for (unsigned int i = 0; i < length; ++i) {
	//Set value of sampler
	_value[i] = v[i];
	//Set node values, with scale transformation if they are bounded
	bool bb = jags_finite(_lower[i]); //bounded below
	bool ba = jags_finite(_upper[i]); //bounded above
	if (bb && ba) {
	    double w = 1.0 / (1 - exp(-v[i]));
	    nodes_value[i] = _lower[i] + w * (_upper[i] - _lower[i]);
	}
	else if (bb) {
	    nodes_value[i] = _lower[i] + exp(v[i]);
	}
	else if (ba) {
	    nodes_value[i] = _upper[i] - exp(v[i]);
	}
	else {
	    nodes_value[i] = v[i];
	}
    }
    setValue(nodes_value, length, _chain);
    delete [] nodes_value;
}

#include <iostream>
void MixSampler::getValue(double *value, unsigned int length)
{
    if (length != value_length()) {
	throw logic_error("Length error in MixSampler::getValue");
    }

    //First copy values from the nodes
    double *vp = value;
    vector<StochasticNode*> const &snodes = nodes();
    for (unsigned int j = 0; j < snodes.size(); ++j) {
	double const *value_j = snodes[j]->value(_chain);
	unsigned int length_j = snodes[j]->length();
	copy(value_j, value_j + length_j, vp);
	vp += length_j;
    }
    
    //Then transform them, if necessary
    for (unsigned int i = 0; i < length; ++i) {
	//std::cout << "(" << _lower[i] << "," << _upper[i] << ") : " << value[i] << "\n";
	bool bb = jags_finite(_lower[i]); //bounded below
	bool ba = jags_finite(_upper[i]); //bounded above
	if (bb && ba) {
	    value[i] = log(value[i] - _lower[i]) - log(_upper[i] - value[i]);
	}
	else if (bb) {
	    value[i] = log(value[i] - _lower[i]); 
	}
	else if (ba) {
	    value[i] = log(_upper[i] - value[i]);
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
    unsigned int length = value_length();
    double *last_value = new double[length];
    copy(_value, _value + length, last_value);

    /* 
       Set up vector of power steps up and down, with uniform
       geometric progression between maximum and minimum
    */
    vector<double> pwr(_nstep);
    for (unsigned int i = 0; i < _nlevels; ++i) {
	pwr[_nstep - 1 - i] = pwr[i] = exp(_log_min_power * (i+1) / _nlevels);
    }

    double log_global_prob = -logFullConditional(_chain);
    for (unsigned int t = 0; t < _nstep; ++t) {
	/* Now calculate the log density under the tempered
	   distribution.  Following Celeux et al (2000) we temper only
	   the likelihood part of the log density */
	//double ldensity0 = logPrior(_chain) + pwr[t] * logLikelihood(_chain);
	double ldensity0 = logFullConditional(_chain);
	double scale = exp(_lscale[t]);
	for (unsigned int j = 0; j < length; ++j) {
	    _value[j] = last_value[j] + rng->normal() * scale;
	}
	propose(_value, length);
	//double ldensity1 = logPrior(_chain) + pwr[t] * logLikelihood(_chain);
	double ldensity1 = logFullConditional(_chain);
	double lprob = ldensity1 - ldensity0;
	log_global_prob -= lprob;
	_prob[t] = exp(lprob);
	if (rng->uniform() <= _prob[t]) {
	    //Accept modified proposal
	    copy(_value, _value + length, last_value);
	}
	else {
	    //FIXME: we only have to use propose on the last step
	    propose(last_value, length); //Revert to previous proposal
	}
    }
    log_global_prob += logFullConditional(_chain);

    delete [] last_value;

    accept(rng, exp(log_global_prob));
}

//debuggin
#include <iostream>
void MixSampler::rescale(double prob, bool accept)
{
    /* Each step up and down has its own scale */
    for (unsigned int i = 0; i < _nstep; ++i) {
	/*
        std::cout << exp(_lscale[i]);
        if (i == _nstep - 1)
	    std::cout << std::endl;
        else
	    std::cout << ",";
	*/
	double pdiff = fmin(_prob[i], 1.0) - _target_prob;
        _lscale[i] +=  pdiff / _n[i]; 

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
