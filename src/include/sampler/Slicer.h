#ifndef SLICER_H_
#define SLICER_H_

#include <sampler/Sampler.h>

class StochasticNode;

/**
 * @short Slice Sampler 
 * 
 * The slice sampler is defined by Neal R (2003) Slice Sampling,
 * Ann. Statist.  31; 3: 705-767.
 *
 * The Slicer class is written with a high level of abstraction to
 * allow it to be easily adapted: for example, a slice sampler can be
 * used to sample from a discrete distribution or from a univariate
 * "slice" of a multivariate distribution.
 */
class Slicer : public Sampler 
{
    double _width;
    bool _adapt;
    unsigned int _max;
    double _sumdiff;
    unsigned int _iter;
    bool accept(double xold, double xnew, double z, double L, double R,
		double lower, double upper);
public:
    /**
     * Constructor for Slice Sampler
     *
     * @param node StochasticNode to sample
     *
     * @param graph Graph containing node within which sampling takes place.
     *
     * @param width Initial width of slice
     *
     * @param max Maximum number of times initial width of slice will
     * increase at each iteration.
     */
    Slicer(std::vector<StochasticNode*> const &nodes, Graph const &graph,
	   double width,  unsigned int max);
    /**
     * Update the current value using the "stepping" method. A subclass
     * of Slicer can implement Sampler#update simply by calling this function.
     */
    void updateStep(RNG *rng);
    /**
     * Update the current value using the "doubling" method. A subclass
     * of Slicer can implement Sampler#update simply by calling this function.
     */
    void updateDouble(RNG *rng);
    /**
     * Returns the current value of the sampler. A Slicer may have it's
     * own value, distinct from the value of the StochasticNode being
     * updated.
     */
    virtual double value() const = 0;
    /**
     * Sets the value of the sampler, and additionally updates all the
     * deterministic descendants. This function is virtual because the
     * value of the sampler and the value of the sampled node must be
     * defined by a subclass of Slicer.
     *
     * @param x value to set
     */
    virtual void setValue(double x) = 0;
    /**
     * Gets the lowest and highest possible values of the sampler
     */
    virtual void getLimits(double *lower, double *upper) const = 0;
    /**
     * Turns off adaptive mode.  The current adaptation test is very
     * basic, and will return true if a minimum number of iterations
     * (50) have taken place.
     */
    bool adaptOff();
    /**
     * Returns the log probability density function of the target
     * distribution at the current value of the sampler. This function
     * is called by updateStep and updateDouble.
     */
    virtual double logDensity() const = 0;
};

#endif /* SLICER_H_ */
