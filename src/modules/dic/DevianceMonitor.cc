#include "DevianceMonitor.h"

#include <graph/StochasticNode.h>

#include <stdexcept>

using std::vector;
using std::logic_error;
using std::string;

namespace dic {

DevianceMonitor::DevianceMonitor(Node const *node, unsigned int start,
                                 unsigned int thin)
    : Monitor("deviance", node, start, thin), _values(node->nchain()), 
      _snode(asStochastic(node))
{
    if (_snode == 0) {
	throw logic_error("Invalid node in DevianceMonitor constructor");
    }
}

unsigned int DevianceMonitor::nchain() const
{
    return node()->nchain();
	
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
    unsigned int nchain = _snode->nchain();
    for (unsigned int ch = 0; ch < nchain; ++ch) {
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
