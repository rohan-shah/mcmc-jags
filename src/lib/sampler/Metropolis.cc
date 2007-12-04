#include <config.h>
#include <sampler/Metropolis.h>
#include <sampler/ParallelDensitySampler.h>
#include <graph/StochasticNode.h>
#include <rng/RNG.h>

#include <algorithm>
#include <stdexcept>

using std::logic_error;
using std::vector;
using std::copy;
using std::min;

static unsigned int addDF(vector<StochasticNode *> const &nodes)
{
    unsigned int d = 0;
    for (unsigned int i = 0; i < nodes.size(); ++i) {
        d += df(nodes[i]);
     }
     return d;
}

static unsigned int addLength(vector<StochasticNode *> const &nodes)
{
    unsigned int d = 0;
    for (unsigned int i = 0; i < nodes.size(); ++i) {
	d += nodes[i]->length();
    }
    return d;
}

Metropolis::Metropolis(vector<StochasticNode*> const &nodes)
    : _adapt(true), _value(0), _last_value(0), _length(0)
{
    _length = addLength(nodes);
    _value = new double[_length];
    _last_value = new double[_length];
}

Metropolis::~Metropolis()
{
    delete [] _value;
    delete [] _last_value;
}

void Metropolis::initialize(ParallelDensitySampler *sampler, unsigned int chain)
{
    /* 
       We can't do this in the constructor because we are relying
       on virtual functions transform and untransform. These are
       defined by a subclass of Metropolis which does not yet
       exist when the constructor is called
    */

    unsigned int n = addLength(sampler->nodes());
    double *node_values = new double[n];

    vector<StochasticNode *> const &nodes = sampler->nodes();
    double *nvp = node_values;
    for (unsigned int i = 0; i < nodes.size(); ++i) {
	double const *nvalue = nodes[i]->value(chain);
	unsigned int nlength = nodes[i]->length();
	copy(nvalue, nvalue + nlength, nvp);
	nvp += nlength;
    }
    untransform(node_values, n, _value, _length);

    delete [] node_values;

    copy(_value, _value + _length, _last_value);
}

void Metropolis::propose(double const *value, unsigned int Nvalue)
{
    if (Nvalue != _length) {
	throw logic_error("Invalid length in Metropolis::propose");
    }
    copy(value, value + Nvalue, _value);
    unsigned int Nnode = _sampler->length();
    double *node_values = new double[Nnode];
    transform(value, Nvalue, node_values, Nnode);
    _sampler->setValue(node_values, Nnode, _chain);
    delete [] node_values;
}

bool Metropolis::accept(RNG *rng, double prob)
{
    bool accept = rng->uniform() <= prob;
    if (accept) {
	//Store current value as last accepted value
	copy(_value, _value + _length, _last_value);
    }
    else {
	//Revert to last accepted value
	propose(_last_value, _length);
    }
    if (_adapt) {
	rescale(min(prob, 1.0));
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
    return _length;
}

double const *Metropolis::value() const
{
    return _value;
}

/*
unsigned int Metropolis::chain() const
{
   return _chain;
}
*/
