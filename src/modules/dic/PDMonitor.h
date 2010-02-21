#ifndef PD_MONITOR_H_
#define PD_MONITOR_H_

#include <model/Monitor.h>
#include <graph/StochasticNode.h>

#include <vector>

class StochasticNode;
class RNG;

namespace dic {

    class PDMonitor : public Monitor {
	std::vector<double> _values;
	std::vector<double> _weights;
	unsigned int _nchain;
	unsigned int _n;
    public:
	PDMonitor(std::vector<StochasticNode const *> const &nodes);
	unsigned int nchain() const;
	std::vector<unsigned int> dim() const;
	std::vector<unsigned int> dim1() const;
	std::vector<double> const &value(unsigned int chain) const;
	void reserve(unsigned int niter);
	SArray dump() const;
	bool poolChains() const;
	bool poolIterations() const;
	void update();
	virtual double divergence(unsigned int k, unsigned int ch1, unsigned int ch2) const;
    };

}

#endif /* PD_MONITOR_H_ */
