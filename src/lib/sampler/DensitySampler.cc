#include <sampler/DensitySampler.h>
#include <sampler/DensityMethod.h>
#include <sampler/Updater.h>

#include <stdexcept>

using std::vector;
using std::logic_error;
using std::string;

DensitySampler::DensitySampler(Updater *updater,
			       vector<DensityMethod *> const &methods)
    : Sampler(updater), _methods(methods)
{
}

DensitySampler::~DensitySampler()
{
    for (unsigned int ch = 0; ch < _methods.size(); ++ch) {
       delete _methods[ch];
    }
}

void DensitySampler::update(std::vector<RNG*> const &rngs)
{
    for (unsigned int ch = 0; ch < rngs.size(); ++ch) {
	_methods[ch]->update(rngs[ch]);
    }
}

bool DensitySampler::adaptOff()
{
    bool pass = true;
    for (unsigned int ch = 0; ch < _methods.size(); ++ch) {
	if (!_methods[ch]->adaptOff())
	    pass = false;
    }
    return pass;
}

bool DensitySampler::isAdaptive() const
{
    for (unsigned int ch = 0; ch < _methods.size(); ++ch) {
	if (_methods[ch]->isAdaptive())
	    return true;
    }
    return false;
}

string DensitySampler::name() const
{
    return _methods[0]->name();
}


