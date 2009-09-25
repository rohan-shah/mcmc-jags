#ifndef REAL_SLICER_H_
#define REAL_SLICER_H_

#include <sampler/Slicer.h>

class StochasticNode;
class Updater;

namespace base {

/**
 * Slice sampler for real-valued distributions
 */
    class RealSlicer : public Slicer 
    {
	Updater const *_updater;
	unsigned int _chain;
    public:
	/**
	 * Constructor for Slice Sampler
	 * @param node Node to sample
	 * @param width Initial width of slice
	 * @param maxwidth Maximal width of slice as a multiple of the width
	 * parameter
	 * @param nburn Length of burnin
	 */
	RealSlicer(Updater const *updater, unsigned int chain,
		   double width = 1, long maxwidth = 10);
	double value() const;
	void setValue(double value);
	void getLimits(double *lower, double *upper) const;
	void update(RNG *rng);
	std::string name() const;
	static bool canSample(StochasticNode const *node);
	double logDensity() const;
    };

}

#endif /* REAL_SLICER_H_ */

