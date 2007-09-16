#include "PDMonitor.h"

#include <graph/StochasticNode.h>
#include <distribution/Distribution.h>
#include <rng/RNG.h>

#include <stdexcept>

using std::vector;
using std::logic_error;
using std::string;

namespace dic {

    PDMonitor::PDMonitor(StochasticNode const *snode, unsigned int start,
			 unsigned int thin, vector<RNG *> const &rngs,
			 unsigned int nrep)
	: Monitor("pD", snode, start, thin), _snode(snode), _rngs(rngs),
	  _nrep(nrep)
    
    {
	if (snode->nchain() < 2) {
	    throw logic_error("PDMonitor needs at least 2 chains");
	}
    }

    unsigned int PDMonitor::nchain() const
    {
	return 1;
    }

    vector<unsigned int> PDMonitor::dim() const
    {
	return vector<unsigned int>(1, 1 + niter());
    }
 
    vector<double> const &PDMonitor::value(unsigned int chain) const
    {
	return _values;
    }

    void PDMonitor::doUpdate()
    {
	unsigned int nchain = _snode->nchain();
	
	Distribution const *dist = _snode->distribution();
	vector<vector<unsigned int> > const  &dims = _snode->parameterDims();
	unsigned int length = _snode->length();
	
	double *rep_values = new double[length];

	double pD = 0;
	for (unsigned int ch = 0; ch < nchain; ++ch) {
	    for (unsigned int i = 0; i < _nrep; ++i) {
		dist->randomSample(rep_values, length, 
				   _snode->parameters(ch), dims, _rngs[ch]);
		
		double lik0 = dist->logLikelihood(rep_values, length,
						  _snode->parameters(ch), dims);
		double lik1 = 0;
		for (unsigned int n = 0; n < nchain; ++n) {
		    if (n != ch) {
			lik1 += dist->logLikelihood(rep_values, length,
						    _snode->parameters(n), 
						    dims);
		    }
		}
		pD += lik0 - lik1/(nchain - 1);
	    }
	}
	_values.push_back(pD/(_nrep * nchain));

	delete [] rep_values;
    }

    void PDMonitor::reserve(unsigned int niter)
    {
	unsigned int N = 1 + niter / thin();
	_values.reserve(_values.size() + N);
    }

}