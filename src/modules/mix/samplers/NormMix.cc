#include <config.h>
#include "NormMix.h"
#include <rng/RNG.h>
#include <util/nainf.h>
#include <sampler/GraphView.h>

#include <cmath>
#include <stdexcept>

class Graph;

using std::vector;
using std::logic_error;
using std::log;
using std::exp;
using std::string;

static void read_bounds(vector<StochasticNode*> const &snodes, 
			unsigned int chain,
			double *lower, double *upper, unsigned int length)
{

    double *lp = lower;
    double *up = upper;
    unsigned int node_length = 0;
    for (unsigned int j = 0; j < snodes.size(); ++j) {
	unsigned int length_j = snodes[j]->length();
	node_length += length_j;
	if (node_length > length) {
	    throw logic_error("Invalid length in read_bounds (NormMix)");
	}
	else {
	    snodes[j]->support(lp, up, length_j, chain);
	    lp += length_j;
	    up += length_j;
	}
    }
}

static vector<double> initialValue(GraphView const *gv, unsigned int chain)
{
    vector<double> ivalue(gv->length());
    gv->getValue(ivalue, chain);
    return ivalue;
}

namespace mix {

    NormMix::NormMix(GraphView const *gv, unsigned int chain)
	: TemperedMetropolis(initialValue(gv, chain)),
	  _gv(gv), _chain(chain)
    {
	unsigned int N = gv->length();
	_lower = new double[N];
	_upper = new double[N];
	read_bounds(_gv->nodes(), 0, _lower, _upper, N);
    }

    NormMix::~NormMix()
    {
	delete [] _lower;
	delete [] _upper;
    }

    void NormMix::step(vector<double>  &x, double step, RNG *rng) const
    {
	for (unsigned int i = 0; i < x.size(); ++i) {
	    bool bb = jags_finite(_lower[i]); //bounded below
	    bool ba = jags_finite(_upper[i]); //bounded above
	    double eps = rng->normal() * step;
	    if (bb && ba) {
		x[i] = log(x[i] - _lower[i]) - log(_upper[i] - x[i]);
		x[i] += eps;
		double w = 1.0 / (1 + exp(-x[i]));
		x[i] = (1 - w) * _lower[i] + w * _upper[i];
	    }
	    else if (bb) {
		x[i] = log(x[i] - _lower[i]); 
		x[i] += eps;
		x[i] = _lower[i] + exp(x[i]);
	    }
	    else if (ba) {
		x[i] = log(_upper[i] - x[i]);
		x[i] += eps;
		x[i] = _upper[i] - exp(x[i]);
	    }
	    else {
		x[i] = x[i] + eps;
	    }
	}
    }

    double NormMix::logJacobian(vector<double> const &x) const
    {
	double lj = 0;
	for (unsigned int i = 0; i < x.size(); ++i) {
	    bool bb = jags_finite(_lower[i]); //bounded below
	    bool ba = jags_finite(_upper[i]); //bounded above
	    if (bb && ba) {
		lj += log(x[i] - _lower[i]) + log(_upper[i] - x[i]);
	    }
	    else if (bb) {
		lj += log(x[i] - _lower[i]); 
	    }
	    else if (ba) {
		lj += log(_upper[i] - x[i]);
	    }
	}
	return lj;
    }

    bool NormMix::canSample(vector<StochasticNode *> const &snodes)
    {
	if (snodes.empty())
	    return false;

	for (unsigned int i = 0; i < snodes.size(); ++i) {
	    if (snodes[i]->isDiscreteValued()) {
		return false;
	    }
	    /* Check that the bounds (if any) of each node are fixed 
	       We rely on parameter transformation for bounded nodes which
	       must remain the same between iterations to guarantee
	       stationarity of the chain
	    */
	    if (!isSupportFixed(snodes[i])) {
		return false;
	    }
	    //Check that all nodes are of full rank
	    if (snodes[i]->length() != snodes[i]->df())
		return false;
	}
	return true;
    }

    string NormMix::name() const
    {
	return "NormMix";
    }

    void NormMix::getValue(vector<double> &x) const 
    {
	_gv->getValue(x, _chain);
    }

    void NormMix::setValue(vector<double> const &x)
    {
	_gv->setValue(x, _chain);
    }

    double NormMix::logDensity() const
    {
        return _gv->logFullConditional(_chain);
    }
}
