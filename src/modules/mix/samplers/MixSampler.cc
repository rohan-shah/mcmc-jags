#include <config.h>
#include "MixSampler.h"
#include <graph/Graph.h>
#include <graph/StochasticNode.h>
#include <graph/GraphMarks.h>
#include <distribution/Distribution.h>
#include <rng/RNG.h>
#include <sarray/util.h>
#include <sarray/nainf.h>
#include <sampler/ParallelDensitySampler.h>

#include <cmath>
#include <algorithm>
#include <stdexcept>

using std::vector;
using std::copy;
using std::logic_error;
using std::invalid_argument;
using std::log;
using std::exp;
using std::min;

#define N_REFRESH 100

//#include <iostream>

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
	    support(lp, up, length_j, snodes[j], chain);
	    lp += length_j;
	    up += length_j;
	}
    }
}

namespace mix {

    MixSampler::MixSampler(vector<StochasticNode*> const &nodes,
			   unsigned int max_level, double delta, 
			   unsigned int nrep)
	: Metropolis(nodes), 
	  _max_level(max_level), 
	  _delta(delta),
	  _nrep(nrep),
	  _level(0),
	  _lower(0), _upper(0),
	  _n(0), 
	  _lstep(max_level+1), 
	  _pmean(max_level+1),
	  _temper(false)
    {
	if (_delta <= 0) {
	    throw invalid_argument("delta must be positive in MixSampler");
	}

	_lstep[0] = -5;
	_pmean[0] = 0;

	unsigned int N = value_length();
	_lower = new double[N];
	_upper = new double[N];
	read_bounds(nodes, 0, _lower, _upper, N);
    }

    MixSampler::~MixSampler()
    {
	delete [] _lower;
	delete [] _upper;
    }

    void MixSampler::transform(double const *v, unsigned int length,
			       double *nv, unsigned int nlength) const
    {
	if (length != value_length() || nlength != length) {
	    throw logic_error("Length error in MixSampler::transform");
	}

	for (unsigned int i = 0; i < length; ++i) {
	    bool bb = jags_finite(_lower[i]); //bounded below
	    bool ba = jags_finite(_upper[i]); //bounded above
	    if (bb && ba) {
		double w = 1.0 / (1 + exp(-v[i]));
		nv[i] = (1 - w) * _lower[i] + w * _upper[i];
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

    void MixSampler::untransform(double const *nv, unsigned int nlength,
				 double *v, unsigned int length) const
    {
	if (length != value_length() || length != nlength) {
	    throw logic_error("Length error in MixSampler::untransform");
	}

	for (unsigned int i = 0; i < length; ++i) {
	    bool bb = jags_finite(_lower[i]); //bounded below
	    bool ba = jags_finite(_upper[i]); //bounded above
	    if (bb && ba) {
		v[i] = log(nv[i] - _lower[i]) - log(_upper[i] - nv[i]);
	    }
	    else if (bb) {
		v[i] = log(nv[i] - _lower[i]); 
	    }
	    else if (ba) {
		v[i] = log(_upper[i] - nv[i]);
	    }
	    else {
		v[i] = nv[i];
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
	    if (!isSupportFixed(snodes[i])) {
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
	copy(value(), value() + length, last_proposal);

	//We first do a non-tempered update

	_temper = false;
	double pmean = 0;
	for (unsigned int i = 0; i < _nrep; ++i) {
	    double lprob = -_sampler->logFullConditional(_chain);
	    double step = exp(_lstep[0]);
	    for (unsigned int j = 0; j < length; ++j) {
		proposal[j] = last_proposal[j] + rng->normal() * step;
	    }
	    propose(proposal, length);
	    lprob += _sampler->logFullConditional(_chain);
	    double prob = min(exp(lprob), 1.0);
	    if(accept(rng, prob)) {
		copy(proposal, proposal + length, last_proposal);
	    }
	    pmean += prob / _nrep;
	}
	_pmean[0] += pmean / N_REFRESH;


	//Now do a tempered update

	_temper = true;

	double lprior0 = _sampler->logPrior(_chain);
	double llik0 = _sampler->logLikelihood(_chain);
    
	unsigned int nstep = 2 * _level;
	vector<double> pwr(nstep + 2);
	for (unsigned int t = 0; t <= _level; ++t) {
	    // NB Operator precedence here! Since t is an unsigned integer
	    // you get something very nasty if you try exp(-t * _delta)
	    pwr[t] = pwr[nstep + 1 - t] = exp(-(t * _delta));
	}

	double log_global_prob = (pwr[1] - pwr[0]) * llik0;

	for (unsigned int t = 1; t <= nstep; ++t) {

	    unsigned int l = (t <= _level) ? t : (nstep + 1 - t);
	    double step = exp(_lstep[l]);

	    pmean = 0;
	    for (unsigned int i = 0; i < _nrep; ++i) {
		// Generate new proposal
		for (unsigned int j = 0; j < length; ++j) {
		    proposal[j] = last_proposal[j] + rng->normal() * step;
		}
		propose(proposal, length);
	    
		// Calculate new prior and likelihood
		double lprior1 = _sampler->logPrior(_chain);
		double llik1 = _sampler->logLikelihood(_chain);
	    
		// Calculate acceptance probability for new proposal
		double lprob = (lprior1 - lprior0) + pwr[t] * (llik1 - llik0);
		double prob = exp(lprob);
		if (rng->uniform() <= prob) {
		    //Accept modified proposal
		    copy(proposal, proposal + length, last_proposal);
		    lprior0 = lprior1;
		    llik0 = llik1;
		}
		pmean += min(prob, 1.0) / _nrep;
	    }
	    log_global_prob += (pwr[t+1] - pwr[t]) * llik0;
	    if (t <= _level) {
		_pmean[t] += pmean / N_REFRESH;
	    }
	}

	propose(last_proposal, length); //Revert to last known good proposal
	accept(rng, exp(log_global_prob));

	delete [] proposal;
	delete [] last_proposal;
    }

    void MixSampler::rescale(double prob)
    {
	if (_temper) {

	    /* Rescale only after a tempered update */

	    _n++;
	    if (_n % N_REFRESH != 0) {
		return;
	    }

	    bool adapted = true;
	    for (unsigned t = 0; t <= _level; ++t) {
		if (_pmean[t] < 0.05) {
		    _lstep[t] -= log(1.50);
		}
		else if (_pmean[t] < 0.15) {
		    _lstep[t] -= log(1.15);
		}
		else if (_pmean[t] > 0.90) {
		    _lstep[t] += log(1.50);
		}
		else if (_pmean[t] > 0.35) {
		    _lstep[t] += log(1.15);
		}
		if (_pmean[t] < 0.15 || _pmean[t] > 0.35) {
		    adapted = false;
		}
		_pmean[t] = 0; //Reset
	    }
	    if (adapted && _level < _max_level) {
		_level++;
		_lstep[_level] = _lstep[_level - 1] + _delta/2;
		_pmean[_level] = 0;
	    }
	}
    }

    bool MixSampler::checkAdaptation() const
    {
	/* True if all up-phase transitions are within target range
	   for acceptance */
	return (_level == _max_level);
    }

}
