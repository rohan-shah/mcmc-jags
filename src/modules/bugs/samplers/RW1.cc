#include <config.h>
#include "RW1.h"

//#include <matrix.h>
//#include <DMNorm.h>
//#include <lapack.h>

#include <graph/StochasticNode.h>
#include <sampler/SingletonGraphView.h>
#include <rng/RNG.h>

#include <cmath>
#include <algorithm>

using std::vector;
using std::copy;
using std::exp;
using std::sqrt;
using std::min;
using std::string;

namespace jags {
namespace bugs {

    RW1::RW1(SingletonGraphView const *gv, 
	     unsigned int chain)
	: Metropolis(vector<double>(gv->length(), 0)),
	  _gv(gv), _chain(chain),
	  _step_adapter(0.1),
	  _pmean(0), _niter(2)
    {
	gv->checkFinite(chain); //Check validity of initial values
    }
    
    RW1::~RW1()
    {
    }
    
    void RW1::update(RNG *rng)
    {
	double logdensity = -_gv->logFullConditional(_chain);
	double step = _step_adapter.stepSize();
	
	double const *xold = _gv->node()->value(_chain);
	unsigned int N = _gv->length();

	vector<double> xnew(N);
	double S = 0;
	for (unsigned int i = 0; i < N; ++i) {
	    double eps = rng->normal();
	    xnew[i] = xold[i] + eps * step;
	    S += xnew[i];
	}
	S /= N;
	for (unsigned int i = 0; i < N; ++i) {
	    xnew[i] -= S;
	}
	
	setValue(xnew);
	logdensity += _gv->logFullConditional(_chain);
	accept(rng, exp(logdensity));
    }

    void RW1::rescale(double p)
    {
	
	_step_adapter.rescale(p);
	
	// We keep a weighted mean estimate of the mean acceptance
	//  probability with the weights in favour of more recent
	//  iterations
	_pmean += 2 * (p - _pmean) / _niter;
	_niter++;
    }

    bool RW1::checkAdaptation() const
    {
	return (_pmean >= 0.15) && (_pmean <= 0.35);
    }
    
    void RW1::getValue(vector<double> &value) const
    {
	double const *v = _gv->node()->value(_chain);
	copy(v, v + _gv->length(), value.begin());
    }
    
    void RW1::setValue(vector<double> const &value)
    {
	_gv->setValue(value, _chain);
    }
    
}
}
