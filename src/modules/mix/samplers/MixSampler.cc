#include <config.h>
#include <MixSampler.h>


static unsigned int getLength(vector<StochasticNode*> const &snodes)
{
    unsigned int length = 0;
    for (unsigned int i = 0; i < snodes.size(); ++i) {
	length += snodes[i]->length();
    }
    return length;
}


MixSampler::MixSampler(vector<StochasticNode *> const &snodes, 
		       Graph const &graph, unsigned int chain,
		       double step)
    : _length(getLength(snodes)), _step(_length), _value(_length)
{
    if (!canSample(snodes, graph)) {
	throw invalid_argument("Can't construct ConjugateNormal sampler");
    }
    
    for (unsigned int i = 0; i < _length; ++i) {
	_step[i] = step;
    }
    unsigned int i = 0;
    for (unsigned int j = 0; j < snodes.size(); ++j) {
	double const value_j = snodes[j]->value();
	for (unsigned int k = 0; k < snodes[j]->length(); ++k) {
	    _value[i++] = value_j[k];
	}
    }
}

void MixSampler::setValue(double const *value, unsigned int length)
{
    if (length != _length) {
	throw logic_error("Length error in MixSampler::setValue");
    }
    
    
}

void MixSampler::update(RNG *rng)
{
    double *oldvalue = new double[_length];
    double *newvalue = new double[_length];
	
    unsigned int i = 0;
    for (unsigned int j = 0; j < nodes().size(); ++j) {
	double const value_j = snodes[j]->value();
	for (unsigned int k = 0; k < snodes[j]->length(); ++k) {
	    oldvalue[i] = value_j[k];
	    newvalue[i] = oldvalue[i] + _step[i] * rng->normal();
	    i++;
	}
    }    

    lik0 = logFullConditional(_chain);
    setValue(newvalue, _length, _chain);
    lik1 = logFullConditional(_chain);
    bool accept == (lik1 > lik0) || (rng->uniform() <= exp(lik1 - lik0));
    if (accept) {
	
    if (lik1 < lik0 && rng->uniform() > exp(lik1 - lik0)) {
	setValue(oldvalue, _length, _chain); //reject proposal
    }
    delete [] oldvalue;
    delete [] newvalue;
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
	vector<bool> fixmask(snodes[i]->parents().size());
	for (unsigned int j = 0; j < snodes[i]->parents().size(); ++j) {
	    fixmask[j] = snodes[i]->parents()[j]->isObserved();
	}
	if (!snodes[i]->distribution()->isSupportFixed(fixmask)) {
	    return false;
	}
    }
    return true;
}

void update(RNG *rng)
{
    unsigned int len = length();
    double *old_value = new double[len];
    copy(old_value, old_value + len, value());
    double *new_value = new double[len];
    copy(new_value, new_value + len, value());

    /* 
       Set up vector of power steps up and down, with uniform
       geometric progression between maximum and minimum
    */
    unsigned int nstep = 2 * _nlevels - 1;
    vector<double> pwr(nstep);
    for (unsigned int t = 0; t < _nlevels - 1; ++t) {
	pwr[nstep - 1 - t] = pwr[t] = exp(_log_min_power * (t+1) / _nlevels);
    }

    double log_global_prob = -logLikelihood();
    for (unsigned int t = 0; t < nstep; ++t) {
	/* Now calculate the log density under the tempered
	   distribution.  Following Celeux et al (2000) we temper only
	   the likelihood part of the log density */
	log_density0 = logPrior() + pwr[t] * logLikelihood();
	for (unsigned int j = 0; j < len; ++j) {
	    new_value[j] = old_value[j] + rng->rnorm() * exp(_lscale[i]);
	}
	propose(new_value, len);
	log_density1 = logPrior() + pwr[t] * logLikelihood();
	double lprob = log_density1 - log_density0;
	log_global_prob -= lprob;
	_prob[i] = (lprob > 0) ? 1.0 : exp(lprob);
	if (rng->uniform() <= _prob[i]) {
	    //Accept modified proposal
	    copy(old_value, old_value + len, new_value);
	}
	else {
	    propose(old_value, len); //Revert to previous proposal
	}
    }
    log_global_prob += logLikelihood();

    accept(rng, exp(log_global_prob));
}

void MixSampler::rescale()
{
    for (unsigned int i = 0; i < nstep; ++i) {
	pdiff = _prob[i] - _target_prob;
        _lscale[i] +=  pdiff / (_n[i] + 1); 
	bool p_over_target = (pdiff >= 0);
	if (p_over_target != _p_over_target[i]) {
	    _p_over_target[i] = p_over_target;
	    _n[i]++;
	}
    }
}

void MixSampler::setValue(double const *value, unsigned int length)
{
    Sampler::setValue(value, length, chain());
}
