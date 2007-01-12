#include <config.h>
#include <sampler/Slicer.h>

Metropolis::Metropolis(vector<StochasticNode *> const &nodes, 
		       Graph const &graph, unsigned int chain,
		       double const *value, unsigned int length,
		       double prob, double scale)
    : Sampler(nodes, graph), _length(length), _prob(prob), 
      _iter(0), _scale(scale), 
      _value(0), _last_value(0), 
{
    /* FIXME: we could check that length is compatible with nodes using
     * df
     */
    _value = new double(_length);
    _last_value = new double(_length);
    for (unsigned int i = 0; i < _length; ++i) {
	_value[i] = _last_value[i] = value[i];
    }
}

Metropolis::~Metropolos()
{
    delete [] _value;
    delete [] _last_value
}

void Metropolis::propose(double const *value, unsigned int length)
{
    if (length != _length) {
	throw logic_error("Invalid length in Metropolis::propose");
    }
    for (unsigned int i = 0; i < _length; ++i) {
	_value[i] == value[i];
    }
    setValue(value, length);
}

bool Metropolis::accept(RNG *rng, double prob)
{
    bool accept = false;
    if (rng->uniform() <= prob) {
	for (unsigned int i = 0; i < _length; ++i) {
	    _last_value[i] = _value[i];
	}
	accept = true;
    }
    else {
	setValue(_last_value, _length);
    }

    if (_adapt) {
	logscale += log(_scale) + (prob - _prob) / (_iter++ + 1);
	_scale = exp(logscale);
    }

    return accept;
}

double Metropolis::iter()
{
    return _iter;
}

void Metropolis::adaptOff()
{
    _adapt = false;
}

double Metropolis::scale()
{
    return _scale;
}
