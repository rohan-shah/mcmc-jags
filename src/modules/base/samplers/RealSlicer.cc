#include <config.h>
#include <sampler/GraphView.h>
#include <graph/StochasticNode.h>
#include <distribution/Distribution.h>

#include "RealSlicer.h"

#include <cmath>
#include <stdexcept>

using std::vector;
using std::string;
using std::logic_error;

namespace base {

    RealSlicer::RealSlicer(GraphView const *gv, unsigned int chain,
			   double width, long maxwidth)
	: Slicer(width, maxwidth), _gv(gv), _chain(chain)
    {
	if (gv->nodes().size() != 1 || 
	    !canSample(gv->nodes().front()))
	{
	    throw logic_error("Invalid RealSlicer");
	}
    }

    bool 
    RealSlicer::canSample(StochasticNode const *node)
    {
	if (node->isDiscreteValued() || node->length() != 1)
	    return false;

	if (node->df() == 0)
	    return false; 

	return true;
    }

    double RealSlicer::value() const
    {
	return _gv->nodes().front()->value(_chain)[0];
    }
 
    void RealSlicer::setValue(double value)
    {
	_gv->setValue(&value, 1, _chain);
    }

    void RealSlicer::getLimits(double *lower, double *upper) const
    {
	_gv->nodes().front()->support(lower, upper, 1, _chain);
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
	return _gv->logFullConditional(_chain);
    }
}
