#include <config.h>
#include <sampler/GibbsSampler.h>
#include <graph/StochasticNode.h>
#include <graph/DeterministicNode.h>

#include <vector>
#include <set>

using std::vector;
using std::set;

GibbsSampler::GibbsSampler(StochasticNode *node, Graph const &graph)
    : Sampler(vector<StochasticNode *>(1, node), graph)
{
}

StochasticNode *GibbsSampler::node()
{
  return nodes().front();
}
