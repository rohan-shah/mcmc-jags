#ifndef REAL_SLICER_H_
#define REAL_SLICER_H_

#include <sampler/Slicer.h>

class StochasticNode;

namespace base {

/**
 * Slice sampler for real-valued distributions
 */
    class RealSlicer : public Slicer 
    {
	bool _fixed_support;
    public:
	/**
	 * Constructor for Slice Sampler
	 * @param node Node to sample
	 * @param width Initial width of slice
	 * @param maxwidth Maximal width of slice as a multiple of the width
	 * parameter
	 * @param nburn Length of burnin
	 */
	RealSlicer(double width = 1, long maxwidth = 10);
	double value() const;
	void setValue(double value);
	void getLimits(double *lower, double *upper) const;
	void update(RNG *rng);
	static bool canSample(StochasticNode const *node);
    };

}

#endif /* REAL_SLICER_H_ */

