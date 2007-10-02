#ifndef PARALLEL_SAMPLER_H_
#define PARALLEL_SAMPLER_H_

#include <sampler/Sampler.h>

class RNG;
class DensityMethod;

/**
 * @short Samples multiple chains in parallel 
 *
 * A ParallelDensitySampler uses a set of DensityMethod update methods
 * to update each chain independently. 
 */
class ParallelDensitySampler : public Sampler
{
    std::vector<DensityMethod*> _methods;
public:
    /**
     * Constructor.
     *
     * @param methods Vector of pointers to  DensityMethod objects,
     * These must be dynamically allocated, as the ParallelDensitySampler
     * will take ownership of them, and will delete them when its
     * destructor is called
     */
    ParallelDensitySampler(std::vector<StochasticNode*> const &nodes,
			   Graph const &graph, 
			   std::vector<DensityMethod*> const &methods);
    ~ParallelDensitySampler();
    void update(std::vector<RNG*> const &);
    bool adaptOff();
};

#endif /* PARALLEL_SAMPLER_H_ */
