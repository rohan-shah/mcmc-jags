#ifndef IMMUTABLE_PARALLEL_SAMPLER_H_
#define IMMUTABLE_PARALLEL_SAMPLER_H_

#include <sampler/Sampler.h>

namespace jags {

    class RNG;
    class ImmutableSampleMethod;

    /**
     * @short Samples multiple chains in parallel
     *
     * Uses an ImmutableSampleMethod object to update each chain
     * independently.
     *
     * In contrast to the MutableParallelSampler, only one sample
     * method is required to update all chains.
     */
    class ImmutableParallelSampler : public Sampler
    {
	ImmutableSampleMethod* _method;
	unsigned int _nchain;
      public:
	/**
	 * Constructor.
	 *
	 * @param gv View of the sample graph, passed directly to the
	 * parent class Sampler.
	 *
	 * @param methods Pointer to an ImmutableSampleMethod This
	 * must be dynamically allocated, as the
	 * ImmutableParallelSampler will take ownership of it, and
	 * will delete it when the destructor is called.
	 */
	ImmutableParallelSampler(GraphView *gv, ImmutableSampleMethod* method);
	~ImmutableParallelSampler();
	/**
	 * Updates all chains in parallel using the update method
	 * provided to the constructor
	 */
	void update(std::vector<RNG*> const &rngs);
	/**
	 * The sampler is not adaptive
	 */
	bool isAdaptive() const;
	/**
	 * This does nothing
	 */
	void adaptOff();
	/**
	 * This always returns true
	 */
	bool checkAdaptation() const;
	/**
	 * 
	 */
	std::string name() const;
    };

} /* namespace jags */

#endif /* IMMUTABLE_PARALLEL_SAMPLER_H_ */
