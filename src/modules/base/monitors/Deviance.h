#ifndef DEVIANCE_H_
#define DEVIANCE_H_

#include <model/Monitor.h>

class StochasticNode;

namespace base {

    class Deviance : public Monitor {
	std::vector<std::vector<double> >  _values; // sampled values
	std::vector<StochasticNode const *> _snodes;
    public:
	Deviance(std::vector<StochasticNode const *> const &nodes);
	std::vector<unsigned int> dim() const;
	std::vector<double> const &value(unsigned int chain) const;
	void update();
	void reserve(unsigned int niter);
	SArray dump() const;
	bool poolChains() const;
	bool poolIterations() const;
    };

}

#endif /* DEVIANCE_H_ */
