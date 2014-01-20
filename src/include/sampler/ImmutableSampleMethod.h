#ifndef IMMUTABLE_SAMPLE_METHOD_H_
#define IMMUTABLE_SAMPLE_METHOD_H_

#include <string>

namespace jags {

class RNG;

/**
 * @short Abstract class for mutable sampling methods 
 */
class ImmutableSampleMethod
{
public:
    virtual ~ImmutableSampleMethod();
    /**
     * Draws another sample from the target distribution
     */
    virtual void update(unsigned int chain, RNG *rng) const = 0;
    /**
     * Indicates whether the sample method has an adaptive mode.
     */
    virtual bool isAdaptive() const = 0;
    /**
     * Turns off adaptive mode
     */
    virtual void adaptOff() = 0;
    /**
     * Checks adaptation 
     */
    virtual bool checkAdaptation() const = 0;
    /**
     * Returns an informative name for the sample method.
     */
    virtual std::string name() const = 0;
};

} /* namespace jags */
    
#endif /* IMMUTABLE_SAMPLE_METHOD_H_ */
