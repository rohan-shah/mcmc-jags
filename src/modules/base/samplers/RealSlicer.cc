#include <config.h>
#include <sampler/Updater.h>
#include <graph/StochasticNode.h>
#include <distribution/Distribution.h>

#include "RealSlicer.h"

#include <cmath>
#include <stdexcept>

using std::vector;
using std::string;
using std::logic_error;

namespace base {

    RealSlicer::RealSlicer(Updater const *updater, unsigned int chain,
			   double width, long maxwidth)
	: Slicer(width, maxwidth), _updater(updater), _chain(chain)
    {
	if (updater->nodes().size() != 1 || 
	    !canSample(updater->nodes().front()))
	{
	    throw logic_error("Invalid RealSlicer");
	}
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
	return _updater->nodes().front()->value(_chain)[0];
    }
 
    void RealSlicer::setValue(double value)
    {
	_updater->setValue(&value, 1, _chain);
    }

    void RealSlicer::getLimits(double *lower, double *upper) const
    {
	support(lower, upper, 1, _updater->nodes().front(), _chain);
    }

    void RealSlicer::update(RNG *rng)
    {
	updateStep(rng);
    }

    string RealSlicer::name() const
    {
	return "RealSlicer";
    }

    double RealSlicer::logDensity() const
    {
	return _updater->logFullConditional(_chain);
    }
}
