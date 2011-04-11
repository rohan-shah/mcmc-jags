#include <config.h>
#include <graph/Node.h>

#include <algorithm>

#include "TraceMonitor.h"

using std::vector;
using std::string;

namespace base {

    TraceMonitor::TraceMonitor(Node const *node)
	: Monitor("trace", node), _values(node->nchain())
    {
    }
    
    void TraceMonitor::update()
    {
	Node const *snode = nodes()[0];
	unsigned int nchain = snode->nchain();
	unsigned int length = snode->length();
	for (unsigned int ch = 0; ch < nchain; ++ch) {
	    double const *node_value = snode->value(ch);
	    for (unsigned int i = 0; i < length; ++i) {
		_values[ch].push_back(node_value[i]);
	    }
	}
    }

    vector<double> const &TraceMonitor::value(unsigned int chain) const
    {
	return _values[chain];
    }

    vector<unsigned int> TraceMonitor::dim() const
    {
	return nodes()[0]->dim();
    }

    void TraceMonitor::reserve(unsigned int niter)
    {
	unsigned int N = niter * nodes()[0]->length();
	for (unsigned int ch = 0; ch < _values.size(); ++ch) {
	    _values[ch].reserve(_values[ch].size() + N);
	}
    }

    bool TraceMonitor::poolChains() const
    {
	return false;
    }

    bool TraceMonitor::poolIterations() const
    {
	return false;
    }
}
