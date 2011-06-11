#include <config.h>
#include <distribution/Distribution.h>
#include <graph/StochasticNode.h>
#include <sampler/ParallelSampler.h>
#include <rng/RNG.h>
#include <util/nainf.h>
#include <graph/NodeError.h>
#include <sampler/GraphView.h>
#include <module/ModuleError.h>

#include "FiniteMethod.h"

#include <cmath>
#include <string>
#include <vector>
#include <algorithm>

using std::string;
using std::vector;
using std::exp;
using std::string;
using std::max;

namespace base {

    FiniteMethod::FiniteMethod(GraphView const *gv, unsigned int chain)
	: _gv(gv), _chain(chain)
    {
	if (gv->nodes().size() != 1)
	    throwLogicError("Invalid FiniteMethod");
	StochasticNode const *snode = gv->nodes().front();
	if (!canSample(snode)) {
	    throwLogicError("Invalid FiniteMethod");
	}

	double lower = 0, upper = 0;
	snode->support(&lower, &upper, 1, 0);

	_lower = static_cast<int>(lower);
	_upper = static_cast<int>(upper);
    }
    
    void FiniteMethod::update(RNG *rng)
    {
	int size = _upper - _lower + 1;
	vector<double> lik(size);

	//Calculate log-likelihood
	double lik_max = JAGS_NEGINF;
	for (int i = 0; i < size; i++) {
	    double ivalue = _lower + i;
	    _gv->setValue(&ivalue, 1, _chain);
	    lik[i] = _gv->logFullConditional(_chain);
	    lik_max = max(lik_max, lik[i]);
	}

	//Transform log-likelihood to likelihood, avoiding overflow
	double liksum = 0;
	for (int i = 0; i < size; ++i) {
	    lik[i] = exp(lik[i] - lik_max);
	    liksum += lik[i];
	}
	
	if (!jags_finite(liksum)) {
	    throwNodeError(_gv->nodes()[0], "Cannot normalize density");
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
	_gv->setValue(&ivalue, 1, _chain);
    }

    bool FiniteMethod::isAdaptive() const
    {
	return false;
    }

    void FiniteMethod::adaptOff()
    {
    }

    bool FiniteMethod::checkAdaptation() const
    {
	return true;
    }
    
    bool FiniteMethod::canSample(StochasticNode const * node)
    {
	//Node must be scalar with discrete-valued distribution of full rank
	if (!node->isDiscreteValued())
	    return false;

	if (node->length() != 1)
	    return false;

	if (node->df() == 0)
	    return false;

	//Support must be fixed
	if (!isSupportFixed(node)) {
	    return false;
	}
	
	//FIXME: If support is fixed it is the same for all chains
	for (unsigned int ch = 0; ch < node->nchain(); ++ch) {
	    //Distribution cannot be unbounded
	    double ulimit = JAGS_NEGINF, llimit = JAGS_POSINF;
	    node->support(&llimit, &ulimit, 1, ch);
	    if (!jags_finite(ulimit) || !jags_finite(llimit))
		return false;

	    //We don't want too many possibilities
	    double n = ulimit - llimit + 1;
	    if (n <= 1 || n > 20) //fixme: totally arbitrary
		return false;
	}
	return true;
    }

    string FiniteMethod::name() const
    {
	return "FiniteMethod";
    }
}
