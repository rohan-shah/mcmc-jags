#include <config.h>
#include <graph/VectorStochasticNode.h>
#include <graph/NodeError.h>
#include <distribution/DistError.h>
#include <distribution/VectorDist.h>
#include <util/nainf.h>
#include <util/dim.h>

#include <vector>
#include <string>
#include <algorithm>

using std::vector;
using std::string;
using std::max;
using std::min;
using std::copy;

static unsigned int mkLength(VectorDist const *dist, 
			     vector<Node const *> const &parents)
{
    /* 
       Calculates length of stochastic node as a function of its
       parents
    */

    if (!dist->checkNPar(parents.size())) {
	throw DistError(dist, "Incorrect number of parameters");
    }
    vector<unsigned int> parameter_lengths(parents.size());
    for (unsigned long j = 0; j < parents.size(); ++j) {
	parameter_lengths[j] = parents[j]->length();
    }
    if (!dist->checkParameterLength(parameter_lengths)) {
	throw DistError(dist, "Non-conforming parameters");
    }
    return dist->length(parameter_lengths);
}

static vector<unsigned int> const &
mkParameterLengths(vector<Node const *> const &parameters) {
    vector<unsigned int>  lengths(parameters.size());
    for (unsigned int j = 0; j < parameters.size(); ++j) {
        lengths[j] = parameters[j]->length();
    }
    return getUnique(lengths);
}

VectorStochasticNode::VectorStochasticNode(VectorDist const *dist, 
					   vector<Node const *> const &params,
					   Node const *lower, Node const *upper)
    : StochasticNode(vector<unsigned int>(1,mkLength(dist, params)), 
		     dist, params, lower, upper),
      _dist(dist), _lengths(mkParameterLengths(params))
{
    if (!dist->checkParameterLength(_lengths)) {
	throw DistError(dist, "Invalid parameter lengths");
    }
}

double VectorStochasticNode::logDensity(unsigned int chain, PDFType type) const
{
    if(!_dist->checkParameterValue(_parameters[chain], _lengths))
	return JAGS_NEGINF;
    
    return _dist->logDensity(_data + _length * chain, _length, type,
			     _parameters[chain], _lengths,
			     lowerLimit(chain), upperLimit(chain));
}

void VectorStochasticNode::deterministicSample(unsigned int chain)
{
    _dist->typicalValue(_data + _length * chain, _length,
			_parameters[chain], _lengths,
			lowerLimit(chain), upperLimit(chain));
}

void VectorStochasticNode::randomSample(RNG *rng, unsigned int chain)
{
    _dist->randomSample(_data + _length * chain, _length, 
			_parameters[chain], _lengths, 
			lowerLimit(chain), upperLimit(chain), rng);
}  

void VectorStochasticNode::truncatedSample(RNG *rng, unsigned int chain,
					   double const *lower,
					   double const *upper)
    
{
    double const *l = lowerLimit(chain);
    double *lv = 0;
    if (l || lower) {
	lv = new double[_length];
	if (l && lower) {
	    for (unsigned int i = 0; i < _length; ++i) {
		lv[i] = min(l[i], lower[i]);
	    }
	}
	else if (l) {
	    copy(l, l + _length, lv);
	}
	else if (lower) {
	    copy(lower, lower + _length, lv);
	}
    }

    double const *u = upperLimit(chain);
    double *uv = 0;
    if (u || upper) {
	uv = new double[_length];
	if (u && upper) {
	    for (unsigned int i = 0; i < _length; ++i) {
		uv[i] = max(u[i], upper[i]);
	    }
	}
	else if (u) {
	    copy(u, u + _length, uv);
	}
	else if (upper) {
	    copy(upper, upper + _length, uv);
	}
    }
    _dist->randomSample(_data + _length * chain, _length, 
			_parameters[chain], _lengths, lv, uv, rng);

    delete [] lv;
    delete [] uv;
}  

bool VectorStochasticNode::checkParentValues(unsigned int chain) const
{
    return _dist->checkParameterValue(_parameters[chain], _lengths);
}

StochasticNode * 
VectorStochasticNode::clone(vector<Node const *> const &parameters,
			    Node const *lower, Node const *upper) const
{
    return new VectorStochasticNode(_dist, parameters, lower, upper);
}

unsigned int VectorStochasticNode::df() const
{
    return _dist->df(_lengths);
}

void VectorStochasticNode::sp(double *lower, double *upper, unsigned int length,
			      unsigned int chain) const
{
    _dist->support(lower, upper, length, _parameters[chain], _lengths);
}
