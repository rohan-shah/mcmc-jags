#include <config.h>
#include <model/TraceMonitor.h>
#include <graph/Node.h>

#include <stdexcept>
#include <algorithm>

using std::max;
using std::min;
using std::runtime_error;
using std::invalid_argument;
using std::logic_error;
using std::vector;

TraceMonitor::TraceMonitor(Node const *node, unsigned int start, 
			   unsigned int thin)
    : Monitor(node), _start(start), _thin(thin), _values(node->nchain()),
      _niter(node->nchain(), 0)
{
    if (thin == 0) {
	throw invalid_argument("Illegal thinning interval");
    }
}

TraceMonitor::~TraceMonitor ()
{
}

unsigned int TraceMonitor::start() const
{
    return _start;
}

unsigned int TraceMonitor::end(unsigned int chain) const
{
    return _start + _thin * _values[chain].size();
}

unsigned int TraceMonitor::thin() const
{
    return _thin;
}

unsigned int TraceMonitor::niter(unsigned int chain) const
{
    return _niter[chain];
}

void TraceMonitor::update(unsigned int iteration, unsigned int chain)
{
    if (iteration < _start || (iteration - _start) % _thin != 0) {
	return;
    }
    if (chain >= _values.size()) {
	throw logic_error("Invalid chain in TraceMonitor::update");
    }

    double const *node_value = node()->value(chain);
    for (unsigned int i = 0; i < node()->length(); i++) {
	_values[chain].push_back(node_value[i]);
    }
    _niter[chain]++;
}

vector<double> const &TraceMonitor::values(unsigned int chain) const
{
    return _values[chain];
}

void TraceMonitor::reserve(unsigned int niter)
{
    for (unsigned int ch = 0; ch < _values.size(); ++ch) {
	_values[ch].reserve((_niter[ch] + niter) * node()->length());
    }
}

bool isSynchronized(TraceMonitor const *monitor)
{
    for (unsigned int ch = 1; ch < monitor->node()->nchain(); ++ch) {
	if (monitor->niter(ch) != monitor->niter(0))
            return false;
    }
    return true;
}
