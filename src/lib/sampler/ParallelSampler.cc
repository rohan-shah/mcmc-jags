#include <sampler/ParallelSampler.h>
#include <sampler/SampleMethod.h>
#include <graph/StochasticNode.h>
#include <sampler/Updater.h>

#include <stdexcept>

using std::vector;
using std::logic_error;
using std::string;

ParallelSampler::ParallelSampler(Updater *updater,
				 vector<SampleMethod *> const &methods)
    : Sampler(updater), _methods(methods)
{
    for (unsigned int i = 0; i < updater->nodes().size(); ++i) {
	if (updater->nodes()[i]->nchain() != methods.size()) {
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

void ParallelSampler::update(std::vector<RNG*> const &rngs)
{
    for (unsigned int ch = 0; ch < rngs.size(); ++ch) {
	_methods[ch]->update(rngs[ch]);
    }
}

bool ParallelSampler::adaptOff()
{
    bool pass = true;
    for (unsigned int ch = 0; ch < _methods.size(); ++ch) {
	if (!_methods[ch]->adaptOff())
	    pass = false;
    }
    return pass;
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
