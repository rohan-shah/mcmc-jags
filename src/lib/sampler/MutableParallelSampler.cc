#include <sampler/MutableParallelSampler.h>
#include <sampler/MutableSampleMethod.h>
#include <graph/StochasticNode.h>
#include <sampler/GraphView.h>

#include <stdexcept>

using std::vector;
using std::logic_error;
using std::string;

namespace jags {

    MutableParallelSampler::MutableParallelSampler(GraphView *gv,
			   vector<MutableSampleMethod *> const &methods)
	: Sampler(gv), _methods(methods)
    {
    }

    MutableParallelSampler::~MutableParallelSampler()
    {
	for (unsigned int ch = 0; ch < _methods.size(); ++ch) {
	    delete _methods[ch];
	}
    }

    void MutableParallelSampler::update(vector<RNG*> const &rngs)
    {
	for (unsigned int ch = 0; ch < rngs.size(); ++ch) {
	    _methods[ch]->update(rngs[ch]);
	}
    }

    void MutableParallelSampler::adaptOff()
    {
	for (unsigned int ch = 0; ch < _methods.size(); ++ch) {
	    _methods[ch]->adaptOff();
	}
    }

    bool MutableParallelSampler::checkAdaptation() const
    {
	for (unsigned int ch = 0; ch < _methods.size(); ++ch) {
	    if (!_methods[ch]->checkAdaptation()) return false;
	}
	return true;
    }


    bool MutableParallelSampler::isAdaptive() const
    {
	for (unsigned int ch = 0; ch < _methods.size(); ++ch) {
	    if (_methods[ch]->isAdaptive())
		return true;
	}
	return false;
    }

    string MutableParallelSampler::name() const
    {
	return _methods[0]->name();
    }

} //namespace jags
