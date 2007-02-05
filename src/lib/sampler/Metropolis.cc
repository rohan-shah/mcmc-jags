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
		       Graph const &graph, unsigned int chain,
		       double const *value, unsigned int length)
    : Sampler(nodes, graph), _chain(chain), _adapt(true), _value(0),
      _last_value(0), _value_length(length)
{
    if (length < addDF(nodes)) {
        throw logic_error("Invalid length in Metropolis constructor");
    }

    _value = new double[length];
    _last_value = new double[length];
    copy(value, value + length, _value);
    copy(value, value + length, _last_value);
}

Metropolis::~Metropolis()
{
    delete [] _value;
    delete [] _last_value;
}

void Metropolis::propose(double const *value, unsigned int Nvalue)
{
    if (Nvalue != _value_length) {
	throw logic_error("Invalid length in Metropolis::propose");
    }
    copy(value, value + Nvalue, _value);
    unsigned int Nnode = length();
    double *node_values = new double[Nnode];
    transformValues(value, Nvalue, node_values, Nnode);
    setValue(node_values, Nnode, _chain);
    delete [] node_values;
}

bool Metropolis::accept(RNG *rng, double prob)
{
    bool accept = rng->uniform() <= prob;
    if (accept) {
	//Store current value as last accepted value
	copy(_value, _value + _value_length, _last_value);
    }
    else {
	//Revert to last accepted value
	propose(_last_value, _value_length);
    }
    if (_adapt) {
	rescale(prob, accept);
    }
    return accept;
}

bool Metropolis::adaptOff()
{
    _adapt = false;
    return checkAdaptation(); 
}

unsigned int Metropolis::value_length() const
{
    return _value_length;
}

double const *Metropolis::value() const
{
    return _value;
}

unsigned int Metropolis::chain() const
{
   return _chain;
}
