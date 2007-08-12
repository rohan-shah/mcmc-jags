#include <config.h>
#include <distribution/Distribution.h>
#include <graph/StochasticNode.h>
#include <sampler/ParallelDensitySampler.h>
#include <rng/RNG.h>
#include <sarray/nainf.h>
#include <graph/NodeError.h>
#include "FiniteSampler.h"

#include <cmath>
#include <string>
#include <stdexcept>
#include <vector>

using std::logic_error;
using std::string;
using std::vector;
using std::exp;

namespace basesamplers {

    void FiniteSampler::initialize(ParallelDensitySampler *sampler,
                                   unsigned int chain)
    {
	StochasticNode const *node = sampler->nodes()[0];
	Distribution const *dist = node->distribution();
    
	double lower = 0, upper = 0;
	dist->support(&lower, &upper, 1, node->parameters(chain), 
		      node->parameterDims());
	_lower = static_cast<int>(lower);
	_upper = static_cast<int>(upper);
    }
	
    void FiniteSampler::update(RNG *rng)
    {
	int size = _upper - _lower + 1;
	double *lik = new double[size];
	double liksum = 0.0;
	for (int i = 0; i < size; i++) {
	    double ivalue = _lower + i;
	    _sampler->setValue(&ivalue, 1, _chain);
	    lik[i] = exp(_sampler->logFullConditional(_chain));
	    liksum += lik[i];
	}

	if (liksum == 0 || !jags_finite(liksum)) {
            Node *node = _sampler->nodes()[0];
	    throw NodeError(node, "Cannot normalize density in FiniteSampler");
	}

	/* Sample */
	double urand = rng->uniform() * liksum;
	int i;
	liksum = 0.0;
	for (i = 0; i < size - 1; i++) {
	    liksum += lik[i];
	    if (liksum > urand) {
		break;
	    }
	}
	double ivalue = _lower + i;
	_sampler->setValue(&ivalue, 1, _chain);
	delete [] lik;
    }

    bool FiniteSampler::adaptOff()
    {
	return true;
    }

}
