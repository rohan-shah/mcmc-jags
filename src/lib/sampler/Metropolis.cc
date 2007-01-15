#include <config.h>
#include <sampler/Metropolis.h>
#include <graph/StochasticNode.h>
#include <rng/RNG.h>

#include <algorithm>
#include <stdexcept>

using std::logic_error;
using std::vector;
using std::copy;

static unsigned int addDF(vector<StochasticNode *> const &nodes)
{
    unsigned int d = 0;
    for (unsigned int i = 0; i < nodes.size(); ++i) {
        d += df(nodes[i]);
     }
     return d;
}

Metropolis::Metropolis(vector<StochasticNode *> const &nodes, 
		       Graph const &graph)
    : Sampler(nodes, graph), _adapt(true), _last_value(0), _size(addDF(nodes))
{
}

Metropolis::~Metropolis()
{
    delete [] _last_value;
}

bool Metropolis::accept(RNG *rng, double prob)
{
    bool accept = rng->uniform() <= prob;
    if (!accept) {
	if (_last_value) {
	    propose(_last_value, _size);
	}
	else {
            _last_value = new double[_size];
	    copy(value(), value() + _size, _last_value);
	}
    }
    if (_adapt) {
	rescale(prob, accept);
    }
    return accept;
}

void Metropolis::adaptOff()
{
    _adapt = false;
}

unsigned int Metropolis::value_length() const
{
    return _size;
}
