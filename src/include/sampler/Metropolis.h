#ifndef METROPOLIS_H_
#define METROPOLIS_H_

#include <sampler/DensityMethod.h>
#include <vector>

class StochasticNode;

/** 
 * @short Metropolis-Hastings Update method
 *
 * This is a base class for Metropolis Hastings samplers.  It provides
 * only basic infrastructure.
 *
 * The Metropolis Hasting sampler has its own value, which is distinct
 * from the value of the sampled nodes.  All member functions of the
 * Metropolis class work in terms of this value.  This is done to
 * allow parameter transformations: for example, if a node only takes
 * positive values, it may be more efficient for the sampler to work
 * on log-transformed values of the sampled node.
 *
 * A Subclass of Metropolis must provide an implementation of the
 * the virtual functions Metropolis#setValue and Metropolis#rescale.
 *
 * The Metropolis class provides no update member function. A subclass
 * of Metropolis must provide this. It should contain one or more
 * calls to Metropolis#propose, then calculate the acceptance
 * probability, and then call the function Metropolis#accept.
 */
class Metropolis : public DensityMethod
{
    bool _adapt;
    double *_value;
    double *_last_value;
    unsigned int _length;
    Metropolis(Metropolis const &);
    Metropolis &operator=(Metropolis const &);
public:
    Metropolis(std::vector<StochasticNode *> const &nodes);
    ~Metropolis();
    void initialize(ParallelDensitySampler *sampler, unsigned int chain);
    /**
     * Returns the current value of the chain. 
     */
    double const *value() const;
    /**
     * Returns the length of the value array. This is the sum of the
     * degrees of freedom of the sampled nodes
     * @see Distribution#df
     */
    unsigned int value_length() const;
    /**
     * Returns the number of the sampled chain 
     */
    unsigned int chain() const;
    /**
     * Sets the value of the sampler. This function calls
     * writeTransfomedValues
     *
     * implement this function, which should set the current value of

     *
     * @param value Pointer to the beginning of an array of values
     * @param length Length of value array
     */
    void propose(double const *value, unsigned int length);
    /**
     * Accept current value with probabilty p. If the current value is
     * not accepted, the sampler reverts to the value at the last
     * successful call to accept.
     *
     * @param rng Random number generator.
     * @param p Probability of accepting the current value.
     *
     * @returns success indicator
     */
    bool accept(RNG *rng, double p);
    /**
     * Rescales the proposal distribution. This function is called by
     * Metropolis#accept when the sampler is in adaptive
     * mode. Rescaling may optionally depend on the acceptance
     * probability.
     *
     * @param p Acceptance probability
     */
    virtual void rescale(double p) = 0;
    /**
     * Turns off adaptive mode
     */
    bool adaptOff();
    /**
     * Tests whether adaptive mode has been successful (e.g. by testing
     * that the acceptance rate lies in an interval around the target 
     * value). This function is called by Metropolis#adaptOff;
     */
    virtual bool checkAdaptation() const = 0;
    /**
     * Transforms the value of the to Node values for the sampled nodes.
     * This function is called by Metropolis#propose
     */
    virtual void transform(double const *v, unsigned int length,
			   double *nv, unsigned int nlength) const = 0;
    /**
     * Transforms the supplied node values to values for the
     * sampled nodes.  This function is called by Metropolis#initialize
     */
    virtual void untransform(double const *nv, unsigned int nlength,
			     double *v, unsigned int length) const = 0;
};

#endif /* METROPOLIS_H_ */
