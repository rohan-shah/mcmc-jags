#include <config.h>
#include "MixSamplerBasic.h"
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
	    throw logic_error("Invalid length in read_bounds (MixSamplerBasic)");
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

MixSamplerBasic::MixSamplerBasic(vector<StochasticNode *> const &snodes, 
		       Graph const &graph, unsigned int chain,
                       double const *value, unsigned int length, 
		       double target_prob)
    : Metropolis(snodes, graph, chain, value, length), 
      _target_prob(target_prob),
      _lower(0), _upper(0),
      _lstep(-10),
      _p_over_target(false),
      _n(INIT_N)
{
    if (!canSample(snodes, graph)) {
	throw invalid_argument("Can't construct Mixture sampler");
    }

    _lower = new double[value_length()];
    _upper = new double[value_length()];
    read_bounds(snodes, chain, _lower, _upper, value_length());
}

MixSamplerBasic::~MixSamplerBasic()
{
    delete [] _lower;
    delete [] _upper;
}

void MixSamplerBasic::transformValues(double const *v, unsigned int length,
				 double *nv, unsigned int nlength) const
{
    if (length != value_length() || nlength != length) {
	throw logic_error("Length error in MixSamplerBasic::transformValues");
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

void MixSamplerBasic::readValues(vector<StochasticNode*> const &snodes,
			    unsigned int chain,
			    double *value, unsigned int length)
{
    unsigned int value_length = 0;
    for (unsigned int i = 0; i < snodes.size(); ++i) {
         value_length += df(snodes[i]);
    }
    if (length != value_length) {
	throw logic_error("Length error in MixSamplerBasic::readValues");
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

bool MixSamplerBasic::canSample(vector<StochasticNode *> const &snodes,
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

void MixSamplerBasic::update(RNG *rng)
{
    unsigned int length = value_length();
    double *proposal = new double[length];
    copy(value(), value() + length, proposal);

    unsigned int ch = chain();
    double lprob = -logFullConditional(ch);
    double scale = exp(_lstep);
    for (unsigned int j = 0; j < length; ++j) {
        proposal[j] += rng->normal() * scale;
    }
    propose(proposal, length);
    delete [] proposal;
    lprob += logFullConditional(ch);
    accept(rng, exp(lprob));
}
    
void MixSamplerBasic::rescale(double prob, bool accept)
{
    double pdiff = fmin(prob, 1.0) - _target_prob;
    _lstep += pdiff / _n;
    if ((pdiff >= 0) != _p_over_target) {
	_p_over_target = !_p_over_target;
	_n++;
    }
}

bool MixSamplerBasic::checkAdaptation() const
{
    return true; //FIXME
}
