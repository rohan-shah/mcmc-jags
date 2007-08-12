#ifndef FINITE_SAMPLER_H_
#define FINITE_SAMPLER_H_

#include <sampler/DensityMethod.h>
class StochasticNode;

namespace basesamplers {

/**
 * Sampler for discrete distributions with support on a finite set.
 */
class FiniteSampler : public DensityMethod {
	int _lower, _upper;
    public:
	void update(RNG *rng);
	void initialize(ParallelDensitySampler *sampler, unsigned int chain);
	//static bool canSample(StochasticNode const *node, Graph const &graph);
	/**
	 * FiniteSampler has no adaptive mode. This function simply
	 * returns true
	 */
	bool adaptOff();
    };

}

#endif /* FINITE_SAMPLER_H_ */
