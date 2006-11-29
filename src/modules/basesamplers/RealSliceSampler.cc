#include <config.h>
#include <graph/StochasticNode.h>
//temporarily required
#include <graph/DeterministicNode.h>
#include <graph/NodeError.h>
#include <distribution/Distribution.h>
#include "RealSliceSampler.h"

#include <cmath>
#include <cfloat>

using std::vector;

RealSliceSampler::RealSliceSampler(StochasticNode *node, Graph const &graph,
                                   unsigned int chain,
				   double width, long maxwidth)
  : Slicer(vector<StochasticNode*>(1,node), graph, width, maxwidth),
    _chain(chain)
{
}

bool 
RealSliceSampler::canSample(StochasticNode const *node, Graph const &graph)
{
  if (node->distribution()->isDiscreteValued() || node->length() != 1)
     return false;

  if (node->distribution()->isDeterministic())
     return false; 

  return true;
}

double RealSliceSampler::value() const
{
  return *nodes().front()->value(_chain);
}
 
void RealSliceSampler::setValue(double value)
{
    Sampler::setValue(&value, 1, _chain);
}

void RealSliceSampler::getLimits(double *lower, double *upper) const
{
  StochasticNode const *snode = nodes().front();
  snode->distribution()->support(lower, upper, 1, snode->parameters(_chain),
                                      snode->parameterDims());
}

void RealSliceSampler::update(RNG *rng)
{
  updateStep(rng);
}

double RealSliceSampler::logFullConditional() const
{
  return Sampler::logFullConditional(_chain);
}
