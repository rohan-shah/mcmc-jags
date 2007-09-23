#include <config.h>
#include <graph/StochasticNode.h>
#include <graph/NodeError.h>
#include <distribution/Distribution.h>
#include <sampler/ParallelDensitySampler.h>

#include "RealSlicer.h"

#include <cmath>

using std::vector;

namespace basesamplers {

    RealSlicer::RealSlicer(double width, long maxwidth)
	: Slicer(width, maxwidth)
    {
    }

    bool 
    RealSlicer::canSample(StochasticNode const *node)
    {
	if (node->distribution()->isDiscreteValued() || node->length() != 1)
	    return false;

	if (df(node) == 0)
	    return false; 

	return true;
    }

    double RealSlicer::value() const
    {
	return _sampler->nodes()[0]->value(_chain)[0];
    }
 
    void RealSlicer::setValue(double value)
    {
	_sampler->setValue(&value, 1, _chain);
    }

    void RealSlicer::getLimits(double *lower, double *upper) const
    {
	StochasticNode const *snode = _sampler->nodes().front();
	support(lower, upper, 1, snode, _chain); 
    }

    void RealSlicer::update(RNG *rng)
    {
	updateStep(rng);
    }
}
