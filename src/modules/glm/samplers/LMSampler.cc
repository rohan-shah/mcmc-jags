#include <config.h>
#include <graph/StochasticNode.h>
#include <graph/LogicalNode.h>
#include <distribution/Distribution.h>
#include <function/Function.h>

#include <map>
#include <string>

#include "LMSampler.h"

using std::map;
using std::string;
using std::vector;
using std::string;


LMSampler::LMSampler(vector<StochasticNode *> const &sample_nodes, 
		     Graph const &graph)
    : Sampler(sample_nodes, graph)
{
    _method.initialize(this, graph);
}

LMSampler::~LMSampler()
{
}

void LMSampler::update(vector<RNG*> const &rngs)
{
    unsigned int nchain = nodes()[0]->nchain();
    for (unsigned int ch = 0; ch < nchain; ++ch) {
	_method.update(this, ch, rngs[ch]);
    }
}

bool LMSampler::isAdaptive() const
{
    return false;
}

bool LMSampler::adaptOff()
{
    return true;
}

string LMSampler::name() const
{
    return _method.name();
}
