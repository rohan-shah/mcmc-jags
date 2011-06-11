#include <sampler/ParallelSampler.h>
#include <sampler/SampleMethod.h>
#include <graph/StochasticNode.h>
#include <sampler/GraphView.h>

#include <stdexcept>

using std::vector;
using std::logic_error;
using std::string;

ParallelSampler::ParallelSampler(GraphView *gv,
				 vector<SampleMethod *> const &methods)
    : Sampler(gv), _methods(methods)
{
    for (unsigned int i = 0; i < gv->nodes().size(); ++i) {
	if (gv->nodes()[i]->nchain() != methods.size()) {
	    throw logic_error("Chain  mismatch in ParallelSampler");
	}
    }
}

ParallelSampler::~ParallelSampler()
{
    for (unsigned int ch = 0; ch < _methods.size(); ++ch) {
       delete _methods[ch];
    }
}

void ParallelSampler::update(vector<RNG*> const &rngs)
{
    for (unsigned int ch = 0; ch < rngs.size(); ++ch) {
	_methods[ch]->update(rngs[ch]);
    }
}

void ParallelSampler::adaptOff()
{
    for (unsigned int ch = 0; ch < _methods.size(); ++ch) {
	_methods[ch]->adaptOff();
    }
}

bool ParallelSampler::checkAdaptation() const
{
    for (unsigned int ch = 0; ch < _methods.size(); ++ch) {
	if (!_methods[ch]->checkAdaptation()) return false;
    }
    return true;
}


bool ParallelSampler::isAdaptive() const
{
    for (unsigned int ch = 0; ch < _methods.size(); ++ch) {
	if (_methods[ch]->isAdaptive())
	    return true;
    }
    return false;
}

string ParallelSampler::name() const
{
    return _methods[0]->name();
}
