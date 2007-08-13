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
    : Monitor(vector<Node const*>(1,node)), _start(start), _thin(thin), 
      _values(node->nchain()), _niter(0)
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

unsigned int TraceMonitor::end() const
{
    return _start + _thin * _values[0].size();
}

unsigned int TraceMonitor::thin() const
{
    return _thin;
}

unsigned int TraceMonitor::niter() const
{
    return _niter;
}

void TraceMonitor::update(unsigned int iteration)
{
    if (iteration < _start || (iteration - _start) % _thin != 0) {
	return;
    }

    Node const *node = nodes()[0];
    unsigned int nchain = node->nchain();
    unsigned int length = node->length();
    for (unsigned int ch = 0; ch < nchain; ++ch) {
	double const *node_value = node->value(ch);
	for (unsigned int i = 0; i < length; ++i) {
	    _values[ch].push_back(node_value[i]);
	}
    }
    _niter++;
}

vector<double> const &TraceMonitor::values(unsigned int chain) const
{
    return _values[chain];
}

void TraceMonitor::reserve(unsigned int niter)
{
    unsigned int length = nodes()[0]->length();
    for (unsigned int ch = 0; ch < _values.size(); ++ch) {
	_values[ch].reserve((_niter + niter) * length);
    }
}
