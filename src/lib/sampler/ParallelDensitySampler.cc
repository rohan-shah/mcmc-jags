#include <sampler/ParallelDensitySampler.h>
#include <sampler/DensityMethod.h>
#include <graph/StochasticNode.h>

#include <stdexcept>

using std::vector;
using std::logic_error;
using std::string;

ParallelDensitySampler::ParallelDensitySampler(
    vector<StochasticNode *> const &nodes, Graph const &graph,
    vector<DensityMethod *> const &methods)
    : Sampler(nodes, graph), _methods(methods)
{
    for (unsigned int i = 0; i < nodes.size(); ++i) {
	if (nodes[i]->nchain() != methods.size()) {
	    throw logic_error("Chain  mismatch in ParallelDensitySampler");
	}
    }
    for (unsigned int ch = 0; ch < methods.size(); ++ch) {
	_methods[ch]->setData(this, ch);
	_methods[ch]->initialize(this, ch);
    }
}

ParallelDensitySampler::~ParallelDensitySampler()
{
    for (unsigned int ch = 0; ch < _methods.size(); ++ch) {
       delete _methods[ch];
    }
}

void ParallelDensitySampler::update(std::vector<RNG*> const &rngs)
{
    for (unsigned int ch = 0; ch < rngs.size(); ++ch) {
	_methods[ch]->update(rngs[ch]);
    }
}

bool ParallelDensitySampler::adaptOff()
{
    bool pass = true;
    for (unsigned int ch = 0; ch < _methods.size(); ++ch) {
	if (!_methods[ch]->adaptOff())
	    pass = false;
    }
    return pass;
}

string ParallelDensitySampler::name() const
{
    return _methods[0]->name();
}
