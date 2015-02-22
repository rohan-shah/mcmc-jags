#ifndef PD_TRACE_H_
#define PD_TRACE_H_

#include <model/Monitor.h>

#include <vector>

namespace jags {

class StochasticNode;

namespace dic {

    class CalKL;

    class PDTrace : public Monitor {
	std::vector<CalKL *> _calkl;
	std::vector<double> _values;
	unsigned int _nchain;
    public:
	PDTrace(std::vector<StochasticNode const *> const &snodes,
		std::vector<CalKL *> const &calkl);
	~PDTrace();
	std::vector<unsigned int> dim() const;
	std::vector<double> const &value(unsigned int chain) const;
	bool poolChains() const;
	bool poolIterations() const;
	void update();
    };

}}

#endif /* PD_TRACE_H_ */
