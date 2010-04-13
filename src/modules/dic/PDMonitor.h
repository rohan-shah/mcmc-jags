#ifndef PD_MONITOR_H_
#define PD_MONITOR_H_

#include <model/Monitor.h>

#include <vector>

class StochasticNode;

namespace dic {

    class CalKL;

    class PDMonitor : public Monitor {
	std::vector<CalKL *> _calkl;
	std::vector<double> _values;
	std::vector<double> _weights;
	double _scale;
	unsigned int _nchain;
	unsigned int _n;
    public:
	PDMonitor(std::vector<StochasticNode const *> const &snodes,
		  std::vector<CalKL *> const &calkl, double scale=1);
	~PDMonitor();
	std::vector<unsigned int> dim() const;
	std::vector<double> const &value(unsigned int chain) const;
	void reserve(unsigned int niter);
	SArray dump() const;
	bool poolChains() const;
	bool poolIterations() const;
	void update();
	virtual double weight(unsigned int k, unsigned int ch) const;
    };

}

#endif /* PD_MONITOR_H_ */
