#ifndef TRACE_MONITOR_H_
#define TRACE_MONITOR_H_

#include <model/Monitor.h>

#include <vector>

namespace base {

/**
 * @short Stores sampled values of a given Node
 */
    class TraceMonitor : public Monitor {
	std::vector<std::vector<double> > _values; // sampled values
    public:
	TraceMonitor(Node const *node, unsigned int start, unsigned int thin);
	void doUpdate();
	std::vector<double> const &value(unsigned int chain) const;
	std::vector<unsigned int> dim() const;
	unsigned int nchain() const;
	void reserve(unsigned int niter);
    };
}

#endif /* TRACE_MONITOR_H_ */
