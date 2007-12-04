#include "DevianceMonitor.h"

#include <graph/StochasticNode.h>

#include <stdexcept>

using std::vector;
using std::logic_error;
using std::string;

namespace dic {

    DevianceMonitor::DevianceMonitor(StochasticNode const *snode, 
				     unsigned int start, unsigned int thin)
	: Monitor("deviance", snode, start, thin), _values(snode->nchain()), 
	  _snode(snode)
    {
    }

    unsigned int DevianceMonitor::nchain() const
    {
	return _snode->nchain();
    }

    vector<unsigned int> DevianceMonitor::dim() const
    {
	return vector<unsigned int>(1, niter());
    }
 
    vector<double> const &DevianceMonitor::value(unsigned int chain) const
    {
	return _values[chain];
    }

    void DevianceMonitor::doUpdate()
    {
	for (unsigned int ch = 0; ch < _values.size(); ++ch) {
	    _values[ch].push_back(-2 * _snode->logDensity(ch));
	}
    }

    void DevianceMonitor::reserve(unsigned int niter)
    {
	unsigned int N = 1 + niter / thin();
	for (unsigned int ch = 0; ch < _values.size(); ++ch) {
	    _values[ch].reserve(_values[ch].size() + N);
	}

    }

}
