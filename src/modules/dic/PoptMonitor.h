#ifndef POPT_MONITOR_H_
#define POPT_MONITOR_H_

#include "PDMonitor.h"

#include <vector>

class StochasticNode;
class CalKL;

namespace dic {

    class PoptMonitor : public PDMonitor {
	std::vector<StochasticNode const*> _snodes;
    public:
	PoptMonitor(std::vector<StochasticNode const *> const &snodes,
		    std::vector<CalKL*> const &calkl);
	double weight(unsigned int k, unsigned int ch) const;
    };

}

#endif /* POPT_MONITOR_H_ */
