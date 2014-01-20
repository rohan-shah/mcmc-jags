#include <config.h>
#include <sampler/ImmutableParallelSampler.h>
//Needed for name
#include <sampler/ImmutableSampleMethod.h>
//Needed for nchain
#include <sampler/GraphView.h>

using std::vector;
using std::string;

namespace jags {

    ImmutableParallelSampler::ImmutableParallelSampler(GraphView *gv, 
	       ImmutableSampleMethod *method)
	: Sampler(gv), _method(method), _nchain(nchain(gv))
    {
    }

    ImmutableParallelSampler::~ImmutableParallelSampler()
    {
	delete _method;
    }

    void ImmutableParallelSampler::update(vector<RNG*> const &rngs)
    {
	for (unsigned int ch = 0; ch < _nchain; ++ch) {
	    _method->update(ch, rngs[ch]);
	}
    }

    bool ImmutableParallelSampler::isAdaptive() const
    {
	return false;
    }

    void ImmutableParallelSampler::adaptOff()
    {
    }

    bool ImmutableParallelSampler::checkAdaptation() const
    {
	return true;
    }

    string ImmutableParallelSampler::name() const
    {
	return _method->name();
    }

}
