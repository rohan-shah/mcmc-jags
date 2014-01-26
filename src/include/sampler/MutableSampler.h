#ifndef MUTABLE_SAMPLER_H_
#define MUTABLE_SAMPLER_H_

#include <sampler/Sampler.h>

namespace jags {

    class RNG;
    class MutableSampleMethod;

    /**
     * @short Samples multiple chains in parallel 
     *
     * A vector of MutableSampleMethod objects is used to update each
     * chain in parallel.
     */
    class MutableSampler : public Sampler
    {
	std::vector<MutableSampleMethod*> _methods;
      public:
	/**
	 * Constructor.
	 *
	 * @param gv View of the sample graph, passed directly to the
	 * parent class Sampler, which takes ownership of it
	 *
	 * @param methods Vector of pointers to MutableSampleMethod
	 * objects, of length equal to the number of chains.  These
	 * must be dynamically allocated, as the
	 * MutableSampler will take ownership of them, and
	 * will delete them when its destructor is called
	 */
	MutableSampler(GraphView *gv, 
			      std::vector<MutableSampleMethod*> const &methods);
	~MutableSampler();
	void update(std::vector<RNG*> const &rngs);
	bool isAdaptive() const;
	void adaptOff();
	bool checkAdaptation() const;
	std::string name() const;
    };

} /* namespace jags */

#endif /* MUTABLE_SAMPLER_H_ */
