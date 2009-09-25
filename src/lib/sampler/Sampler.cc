#include <config.h>
#include <sampler/Sampler.h>
#include <sampler/Updater.h>

using std::vector;

Sampler::Sampler(Updater *updater)
    : _updater(updater)
{
}

Sampler::~Sampler()
{
    delete _updater;
}

vector<StochasticNode*> const &Sampler::nodes() const
{
    return _updater->nodes();
}
