#include <config.h>
#include <sampler/GraphView.h>
#include <graph/StochasticNode.h>
#include <distribution/Distribution.h>
#include <module/ModuleError.h>

#include "RealSlicer.h"

#include <cmath>

using std::vector;
using std::string;

namespace base {

    RealSlicer::RealSlicer(GraphView const *gv, unsigned int chain,
			   double width, long maxwidth)
	: Slicer(width, maxwidth), _gv(gv), _chain(chain)
    {
	if (gv->nodes().size() != 1 || 
	    !canSample(gv->nodes().front()))
	{
	    throwLogicError("Invalid RealSlicer");
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
	if (!updateStep(rng)) {
	    switch(state()) {
	    case SLICER_POSINF:
		throwNodeError(_gv->nodes().front(),
			       "Slicer stuck at value with infinite density");
		break;
	    case SLICER_NEGINF:
		throwNodeError(_gv->nodes().front(),
			       "Current value is inconsistent with data");
		break;
	    case SLICER_OK:
		break;
	    }
	}
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
