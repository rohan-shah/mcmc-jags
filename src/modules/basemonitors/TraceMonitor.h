#ifndef TRACE_MONITOR_H_
#define TRACE_MONITOR_H_

#include <model/Monitor.h>
#include <vector>

class Node;

namespace basemonitors {
/**
 * @short Stores sampled values of a given Node
 */
    class TraceMonitor : public Monitor {
	std::vector<std::vector<double> > _values; // sampled values
    public:
	/**
	 * Constructs a TraceMonitor. 
	 * @param node Node to sample
	 * @param start Iteration at which to start monitoring
	 * @param thin Thinning interval for monitor.  The monitor only
	 * stores iterations for which (iteration - start) % thin == 0.
	 */
	TraceMonitor(Node const *node, unsigned int start, unsigned int thin);
	void doUpdate();
	std::vector<double> const &value(unsigned int chain) const;
	std::vector<unsigned int> dim() const;
	unsigned int nchain() const;
	void reserve(unsigned int niter);
    };
}

#endif
