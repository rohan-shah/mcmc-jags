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
	TraceMonitor(Node const *node);
	void update();
	std::vector<double> const &value(unsigned int chain) const;
	std::vector<unsigned int> dim() const;
	void reserve(unsigned int niter);
	bool poolChains() const;
	bool poolIterations() const;
    };
}

#endif /* TRACE_MONITOR_H_ */
