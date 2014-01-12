#ifndef FINITE_METHOD_H_
#define FINITE_METHOD_H_

#include <sampler/SampleMethod.h>

namespace jags {
namespace base {

/**
 * Sampler for discrete distributions with support on a finite set.
 */
    class FiniteMethod : public SampleMethod {
	SingletonGraphView const *_gv;
	const unsigned int _chain;
	const int _lower, _upper;
    public:
	FiniteMethod(SingletonGraphView const *gv, unsigned int chain);
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

}}

#endif /* FINITE_METHOD_H_ */
