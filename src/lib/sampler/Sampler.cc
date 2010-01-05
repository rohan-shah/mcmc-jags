#include <config.h>
#include <sampler/Sampler.h>
#include <sampler/GraphView.h>

using std::vector;

Sampler::Sampler(GraphView *gv)
    : _gv(gv)
{
}

Sampler::~Sampler()
{
    delete _gv;
}

vector<StochasticNode*> const &Sampler::nodes() const
{
    return _gv->nodes();
}
