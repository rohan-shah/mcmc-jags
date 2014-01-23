#ifndef FINITE_METHOD_H_
#define FINITE_METHOD_H_

#include <sampler/SampleMethod.h>

namespace jags {
    namespace base {

	/**
	 * Sampler for discrete distributions with support on a finite set.
	 */
	class FiniteMethod : public ImmutableSampleMethod {
	    SingletonGraphView const *_gv;
	    const int _lower, _upper;
	  public:
	    FiniteMethod(SingletonGraphView const *gv);
	    void update(unsigned int chain, RNG *rng);
	    std::string name() const;
	    static bool canSample(StochasticNode const *snode);
	};
	
    }
}

#endif /* FINITE_METHOD_H_ */
