#ifndef DENSITY_METHOD_H_
#define DENSITY_METHOD_H_

#include <string>

class RNG;

/**
 * @short Abstract class for sampling methods based on the probability
 * density function of the target distribution.
 */
class DensityMethod
{
public:
    DensityMethod();
    virtual ~DensityMethod();
    /**
     * Draws another sample from the target distribution
     */
    virtual void update(RNG *rng) = 0;
    /**
     * Turns off adaptive mode, returning true if an adaptation test is
     * passed.
     */
    virtual bool adaptOff() = 0;
    /**
     * Indicates whether the update method has an adaptive mode
     */
    virtual bool isAdaptive() const = 0;
    /**
     * Returns an informative name for the update method
     */
    virtual std::string name() const = 0;
};
    
#endif /* DENSITY_METHOD_H_ */
