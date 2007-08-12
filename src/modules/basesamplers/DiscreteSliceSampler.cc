#include <config.h>
#include <graph/StochasticNode.h>
#include <graph/NodeError.h>
#include <distribution/Distribution.h>
#include <sampler/ParallelDensitySampler.h>

#include "DiscreteSliceSampler.h"

#include <cmath>

using std::floor;
using std::vector;

namespace basesamplers {

    DiscreteSlicer::DiscreteSlicer(double width, long ndoubles)
	: Slicer(width, ndoubles), _x(0)
    {
    }

/* FIXME: BELONGS WITH FACTORY OBJECT
 *
 bool DiscreteSlicer::canSample(StochasticNode const *node,
 Graph const &graph)
 {
 if (!node->distribution()->isDiscreteValued() ||
 node->length() != 1)
 return false;

 if (df(node) == 0)
 return false;

 return true;
 }

*/
    void DiscreteSlicer::initialize(ParallelDensitySampler *sampler,
                                    unsigned int chain)
    {
	_x = sampler->nodes()[0]->value(chain)[0];
    }

    void DiscreteSlicer::setValue(double x)
    {
	_x = x;
	x = floor(x);
	_sampler->setValue(&x, 1, _chain);
	/*
	  sampler->nodes().front()->setValue(&x, 1, chain);
	  vector<Node*> const &dc = sampler->deterministicChildren();
	  for (vector<Node*>::const_iterator i(dc.begin()); i != dc.end(); ++i) 
	  {
	  (*i)->deterministicSample(_chain);
	  }
	*/
    }
  
    double DiscreteSlicer::value() const
    {
	return _x;
    }

    void DiscreteSlicer::getLimits(double *lower, double *upper) const
    {
	StochasticNode const *snode = _sampler->nodes().front();
	snode->distribution()->support(lower, upper, 1, 
				       snode->parameters(_chain),
				       snode->parameterDims());
	*upper += 1;
    }

    void DiscreteSlicer::update(RNG *rng)
    {
	updateDouble(rng);
    }

}
