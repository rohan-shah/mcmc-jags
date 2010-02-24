#ifndef DEVIANCE_MONITOR_H_
#define DEVIANCE_MONITOR_H_

#include <model/Monitor.h>

class StochasticNode;

namespace dic {

    class DevianceMonitor : public Monitor {
	std::vector<double>  _values; 
	std::vector<StochasticNode const *> _snodes;
	unsigned int _n;
    public:
	DevianceMonitor(std::vector<StochasticNode const *> const &nodes);
	std::vector<unsigned int> dim() const;
	std::vector<double> const &value(unsigned int chain) const;
	void update();
	void reserve(unsigned int niter);
	bool poolChains() const;
	bool poolIterations() const;
    };

}

#endif /* DEVIANCE_MONITOR_H_ */
