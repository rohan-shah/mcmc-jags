#ifndef DENSITY_METHOD_H_
#define DENSITY_METHOD_H_

class ParallelDensitySampler;
class RNG;

/*
 * Updates a single chain of a ParallelDensitySampler using a method that
 * depends only on the public member functions of ParallelDensitySampler.
 * Typical DensityMethods use only the log density of the sampled
 * nodes by calling Sampler#logFullConditional.
 */
class DensityMethod
{
protected:
    ParallelDensitySampler *_sampler;
    unsigned int _chain;
public:
    DensityMethod();
    virtual ~DensityMethod();
    /**
     * Sets the values of the protected data members _sampler and _chain
     * so that subclasses of DensityMethod can access them. This
     * function is called by ParallelDensitySampler
     */
    void setData(ParallelDensitySampler *sampler, unsigned int chain);
    /**
     * A subclass of DensityMethod may optionally set data members
     * when this function is called. The default does nothing.
     */
    virtual void initialize(ParallelDensitySampler *sampler, unsigned int chain);
    /**
     * Updates the sampler and chain defined by the last call to setData
     */
    virtual void update(RNG *rng) = 0;
    /**
     * Turn of adaptive mode of all the update methods
     *
     * @see Sampler#adaptOff
     */
    virtual bool adaptOff() = 0;
};
    
#endif /* DENSITY_METHOD_H_ */
