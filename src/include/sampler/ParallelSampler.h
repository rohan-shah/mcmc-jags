#ifndef PARALLEL_SAMPLER_H_
#define PARALLEL_SAMPLER_H_

#include <sampler/Sampler.h>

class RNG;
class SampleMethod;

/**
 * @short Samples multiple chains in parallel 
 *
 * A ParallelSampler uses a vector of SampleMethod objects to update
 * each chain independently.
 */
class ParallelSampler : public Sampler
{
    std::vector<SampleMethod*> _methods;
public:
    /**
     * Constructor.
     *
     * @param methods Vector of pointers to SampleMethod objects,
     * These must be dynamically allocated, as the ParallelSampler
     * will take ownership of them, and will delete them when its
     * destructor is called
     */
    ParallelSampler(GraphView *gv, std::vector<SampleMethod*> const &methods);
    ~ParallelSampler();
    void update(std::vector<RNG*> const &rngs);
    bool isAdaptive() const;
    void adaptOff();
    bool checkAdaptation() const;
    std::string name() const;
};

#endif /* PARALLEL_SAMPLER_H_ */
