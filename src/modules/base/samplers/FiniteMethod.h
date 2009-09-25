#ifndef FINITE_METHOD_H_
#define FINITE_METHOD_H_

#include <sampler/SampleMethod.h>

namespace base {

/**
 * Sampler for discrete distributions with support on a finite set.
 */
    class FiniteMethod : public SampleMethod {
	Updater const *_updater;
	unsigned int _chain;
	int _lower, _upper;
    public:
	FiniteMethod(Updater const *updater, unsigned int chain);
	void update(RNG *rng);
	/**
	 * FiniteMethod is not adaptive. This function returns false.
	 */
	bool isAdaptive() const;
	/**
	 * FiniteMethod has no adaptive mode. This function simply
	 * returns true
	 */
	bool adaptOff();
	std::string name() const;
	static bool canSample(StochasticNode const *snode);
    };
}

#endif /* FINITE_METHOD_H_ */
