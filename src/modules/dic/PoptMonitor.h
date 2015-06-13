#ifndef POPT_MONITOR_H_
#define POPT_MONITOR_H_

#include "PDMonitor.h"

#include <vector>

class StochasticNode;
class CalKL;

namespace jags {
namespace dic {

    class PoptMonitor : public PDMonitor {
	std::vector<StochasticNode const*> _snodes;
    public:
	PoptMonitor(std::vector<StochasticNode const *> const &snodes,
		    std::vector<RNG*> const &rngs, unsigned int nrep);
	double weight(StochasticNode const *snode, unsigned int ch) const;
    };

}}

#endif /* POPT_MONITOR_H_ */
