#ifndef SAMPLE_METHOD_H_
#define SAMPLE_METHOD_H_

#include <string>

class RNG;

/**
 * @short Abstract class for sampling methods 
 */
class SampleMethod
{
public:
    virtual ~SampleMethod();
    /**
     * Draws another sample from the target distribution
     */
    virtual void update(RNG *rng) = 0;
    /**
     * Indicates whether the sample method has an adaptive mode.
     */
    virtual bool isAdaptive() const = 0;
    /**
     * Turns off adaptive mode, returning true if an adaptation test is
     * passed.
     */
    virtual bool adaptOff() = 0;
    /**
     * Returns an informative name for the sample method.
     */
    virtual std::string name() const = 0;
};
    
#endif /* SAMPLE_METHOD_H_ */
