#ifndef FINITE_METHOD_H_
#define FINITE_METHOD_H_

#include <sampler/SampleMethod.h>

namespace base {

/**
 * Sampler for discrete distributions with support on a finite set.
 */
    class FiniteMethod : public SampleMethod {
	GraphView const *_gv;
	unsigned int _chain;
	int _lower, _upper;
    public:
	FiniteMethod(GraphView const *gv, unsigned int chain);
	void update(RNG *rng);
	/**
	 * FiniteMethod is not adaptive. This function returns false.
	 */
	bool isAdaptive() const;
	void adaptOff();
	bool checkAdaptation() const;
	std::string name() const;
	static bool canSample(StochasticNode const *snode);
    };
}

#endif /* FINITE_METHOD_H_ */
