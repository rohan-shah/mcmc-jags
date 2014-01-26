#include <sampler/MutableSampler.h>
#include <sampler/MutableSampleMethod.h>
#include <graph/StochasticNode.h>
#include <sampler/GraphView.h>

#include <stdexcept>

using std::vector;
using std::logic_error;
using std::string;

namespace jags {

    MutableSampler::MutableSampler(GraphView *gv,
			   vector<MutableSampleMethod *> const &methods)
	: Sampler(gv), _methods(methods)
    {
    }

    MutableSampler::~MutableSampler()
    {
	for (unsigned int ch = 0; ch < _methods.size(); ++ch) {
	    delete _methods[ch];
	}
    }

    void MutableSampler::update(vector<RNG*> const &rngs)
    {
	for (unsigned int ch = 0; ch < rngs.size(); ++ch) {
	    _methods[ch]->update(rngs[ch]);
	}
    }

    void MutableSampler::adaptOff()
    {
	for (unsigned int ch = 0; ch < _methods.size(); ++ch) {
	    _methods[ch]->adaptOff();
	}
    }

    bool MutableSampler::checkAdaptation() const
    {
	for (unsigned int ch = 0; ch < _methods.size(); ++ch) {
	    if (!_methods[ch]->checkAdaptation()) return false;
	}
	return true;
    }


    bool MutableSampler::isAdaptive() const
    {
	for (unsigned int ch = 0; ch < _methods.size(); ++ch) {
	    if (_methods[ch]->isAdaptive())
		return true;
	}
	return false;
    }

    string MutableSampler::name() const
    {
	return _methods[0]->name();
    }

} //namespace jags
