#include <config.h>
#include <graph/StochasticNode.h>
//fixme: temporarily needed
#include <graph/DeterministicNode.h>
#include <graph/NodeError.h>
#include <distribution/Distribution.h>

#include "DiscreteSliceSampler.h"

#include <cmath>

using std::floor;
using std::vector;

DiscreteSliceSampler::DiscreteSliceSampler(StochasticNode *node, 
					   Graph const &graph,
                                           unsigned int chain,
					   double width, long ndoubles)
  : Slicer(vector<StochasticNode*>(1, node), graph, width, ndoubles), 
    _chain(chain), _x(*node->value(chain))
{
}

bool DiscreteSliceSampler::canSample(StochasticNode const *node,
				     Graph const &graph)
{
  if (!node->distribution()->isDiscreteValued() ||
	  node->length() != 1)
     return false;

  if (df(node) == 0)
      return false;

  return true;
}

void DiscreteSliceSampler::setValue(double x)
{
  _x = x;
  nodes().front()->setValue(&_x, 1, _chain);
  vector<DeterministicNode*> const &dc = deterministicChildren();
  for (vector<DeterministicNode*>::const_iterator i(dc.begin()); 
       i != dc.end(); ++i) 
    {
      (*i)->deterministicSample(_chain);
    }
}
  
double DiscreteSliceSampler::value() const
{
  return _x;
}

void DiscreteSliceSampler::getLimits(double *lower, double *upper) const
{
  StochasticNode const *snode = nodes().front();
  snode->distribution()->support(lower, upper, 1, snode->parameters(_chain),
		  	       snode->parameterDims());
}

void DiscreteSliceSampler::update(RNG *rng)
{
  updateDouble(rng);
}

double DiscreteSliceSampler::logFullConditional() const
{
  return Sampler::logFullConditional(_chain);
}
