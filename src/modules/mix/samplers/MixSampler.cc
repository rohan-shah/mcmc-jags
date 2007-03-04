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

#define N_REFRESH 100
#define NREP 1

#include <iostream>

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
		       unsigned int max_level, double delta)
    : Metropolis(snodes, graph, chain, value, length), 
      _max_level(max_level), 
      _delta(delta),
      _level(0),
      _lower(0), _upper(0),
      _n(0), 
      _lstep(max_level+1), 
      _pmean(max_level+1),
      _temper(false)
{
    if (!canSample(snodes, graph)) {
	throw invalid_argument("Can't construct Mixture sampler");
    }

    _lstep[0] = -5;
    _pmean[0] = 0;

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

#include <iostream>
void MixSampler::update(RNG *rng)
{
    unsigned int length = value_length();
    double *proposal = new double[length];
    double *last_proposal = new double[length];
    copy(value(), value() + length, last_proposal);

    unsigned int ch = chain();

    //We first do a non-tempered update

    _temper = false;

    double lprob = -logFullConditional(ch);
    double step = exp(_lstep[0]);
    for (unsigned int j = 0; j < length; ++j) {
        proposal[j] = last_proposal[j] + rng->normal() * step;
    }
    propose(proposal, length);
    lprob += logFullConditional(ch);
    double prob = fmin(exp(lprob), 1);
    if(accept(rng, prob)) {
	copy(proposal, proposal + length, last_proposal);
    }
    _pmean[0] += prob / N_REFRESH;

    //Now do a tempered update

    _temper = true;

    double lprior0 = logPrior(ch);
    double llik0 = logLikelihood(ch);
    
    unsigned int nstep = 2 * _level;
    vector<double> pwr(nstep + 2);
    for (unsigned int t = 0; t <= _level; ++t) {
        // NB Operator precedence here!
	pwr[t] = pwr[nstep + 1 - t] = exp(-(t * _delta));
    }

    double log_global_prob = (pwr[1] - pwr[0]) * llik0;

    for (unsigned int t = 1; t <= nstep; ++t) {

	unsigned int l = (t <= _level) ? t : (nstep + 1 - t);
	double step = exp(_lstep[l]);

	double pmean = 0;
	for (unsigned int i = 0; i < NREP; ++i) {
	    // Generate new proposal
	    for (unsigned int j = 0; j < length; ++j) {
		proposal[j] = last_proposal[j] + rng->normal() * step;
	    }
	    propose(proposal, length);
	    
	    // Calculate new prior and likelihood
	    double lprior1 = logPrior(ch);
	    double llik1 = logLikelihood(ch);
	    
	    // Calculate acceptance probability for new proposal
	    double lprob = (lprior1 - lprior0) + pwr[t] * (llik1 - llik0);
	    double prob = exp(lprob);
	    if (rng->uniform() <= prob) {
		//Accept modified proposal
		copy(proposal, proposal + length, last_proposal);
		lprior0 = lprior1;
		llik0 = llik1;
	    }
	    pmean += fmin(prob, 1) / NREP;
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

void MixSampler::rescale(double prob, bool accept)
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
	    std::cout << "Moving to level " << _level << "\n";
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
