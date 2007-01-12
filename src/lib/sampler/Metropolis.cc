#include <config.h>
#include <sampler/Metropolis.h>
#include <rng/RNG.h>

#include <algorithm>
#include <stdexcept>

using std::logic_error;
using std::vector;
using std::copy;

Metropolis::Metropolis(vector<StochasticNode *> const &nodes, 
		       Graph const &graph, unsigned int chain,
		       double const *value, unsigned int length)
    : Sampler(nodes, graph), _chain(chain), _length(length), _adapt(true), 
      _value(0), _last_value(0)
{
    /* FIXME: we could check that length is compatible with nodes using
     * df
     */
    _value = new double[_length];
    _last_value = new double[_length];
    copy(value, value + _length, _value);
    copy(value, value + _length, _last_value);
}

Metropolis::~Metropolis()
{
    delete [] _value;
    delete [] _last_value;
}

void Metropolis::propose(double const *value, unsigned int length)
{
    if (length != _length) {
	throw logic_error("Invalid length in Metropolis::propose");
    }
    copy(value, value + length, _value);
    setValue(value, length);
}

bool Metropolis::accept(RNG *rng, double prob)
{
    bool accept = rng->uniform() <= prob;
    if (accept) {
	copy(_value, _value + _length, _last_value);
    }
    else {
	setValue(_last_value, _length);
    }
    if (_adapt) {
	rescale(prob, accept);
    }
    return accept;
}

double const * Metropolis::value() const
{
    return _value;
}

unsigned int Metropolis::length() const
{
    return _length;
}

void Metropolis::adaptOff()
{
    _adapt = false;
}
