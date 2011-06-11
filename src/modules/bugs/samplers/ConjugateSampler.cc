#include <config.h>
#include "ConjugateSampler.h"
//Needed for nchain
#include <sampler/GraphView.h>

using std::vector;
using std::string;

namespace bugs {

ConjugateSampler::ConjugateSampler(GraphView *gv, ConjugateMethod *method)
    : Sampler(gv), _method(method), _nchain(nchain(gv))
{
}

ConjugateSampler::~ConjugateSampler()
{
    delete _method;
}

void ConjugateSampler::update(vector<RNG*> const &rngs)
{
    for (unsigned int ch = 0; ch < _nchain; ++ch) {
	_method->update(ch, rngs[ch]);
    }
}

bool ConjugateSampler::isAdaptive() const
{
    return false;
}

void ConjugateSampler::adaptOff()
{
}

bool ConjugateSampler::checkAdaptation() const
{
    return true;
}

string ConjugateSampler::name() const
{
    return _method->name();
}

}
