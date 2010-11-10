#include <config.h>
#include "ConjugateSampler.h"
//Needed for nchain
#include <sampler/GraphView.h>

using std::vector;
using std::string;

ConjugateSampler::ConjugateSampler(GraphView *gv, ConjugateMethod *method)
    : Sampler(gv), _method(method), _nchain(nchain(gv))
{
}

ConjugateSampler::~ConjugateSampler()
{
    delete _method;
}

bool ConjugateSampler::update(vector<RNG*> const &rngs)
{
    for (unsigned int ch = 0; ch < _nchain; ++ch) {
	if (!_method->update(ch, rngs[ch]))
	    return false;
    }
    return true;
}

bool ConjugateSampler::isAdaptive() const
{
    return false;
}

bool ConjugateSampler::adaptOff()
{
    return true;
}

string ConjugateSampler::name() const
{
    return _method->name();
}
