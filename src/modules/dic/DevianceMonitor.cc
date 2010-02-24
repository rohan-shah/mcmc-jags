#include "DevianceMonitor.h"

#include <graph/StochasticNode.h>

#include <stdexcept>
#include <algorithm>

using std::vector;
using std::logic_error;
using std::string;
using std::copy;

static vector<Node const *> toNodeVec(vector<StochasticNode const *> const &s)
{
    vector<Node const *> ans;
    copy (s.begin(), s.end(), ans.begin());
    return ans;
}

namespace dic {

    DevianceMonitor::DevianceMonitor(vector<StochasticNode const *> const &s)
	: Monitor("mean", toNodeVec(s)), _values(s.size(),0), _snodes(s), _n(0)
    {
    }

    vector<unsigned int> DevianceMonitor::dim() const
    {
	return vector<unsigned int>(1, _values.size());
    }

    bool DevianceMonitor::poolChains() const
    {
	return true;
    }

    bool DevianceMonitor::poolIterations() const
    {
	return true;
    }

    vector<double> const &DevianceMonitor::value(unsigned int chain) const
    {
	return _values;
    }

    void DevianceMonitor::update()
    {
	_n++;
	for (unsigned int i = 0; i < _snodes.size(); ++i) {
	    double loglik = 0;
	    unsigned int nchain = _snodes[i]->nchain();
	    for (unsigned int ch = 0; ch < nchain; ++ch) {
		loglik += _snodes[i]->logDensity(ch) / nchain;
	    }
	    _values[i] += (-2*loglik - _values[i])/_n;
	}
    }

    void DevianceMonitor::reserve(unsigned int niter)
    {
    }
    
}
