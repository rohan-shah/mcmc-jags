#ifndef IMMUTABLE_SAMPLER_H_
#define IMMUTABLE_SAMPLER_H_

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
     * In contrast to the MutableSampler, only one sample
     * method is required to update all chains.
     */
    class ImmutableSampler : public Sampler
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
	 * ImmutableSampler will take ownership of it, and
	 * will delete it when the destructor is called.
	 */
	ImmutableSampler(GraphView *gv, ImmutableSampleMethod* method);
	~ImmutableSampler();
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

#endif /* IMMUTABLE_SAMPLER_H_ */
