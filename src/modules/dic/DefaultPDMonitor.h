#ifndef DEFAULT_PD_MONITOR_H_
#define DEFAULT_PD_MONITOR_H_

#include "PDMonitor.h"

#include <model/Monitor.h>
#include <graph/StochasticNode.h>
#include <vector>

class StochasticNode;
class RNG;

namespace dic {

    class DefaultPDMonitor : public PDMonitor {
	std::vector<StochasticNode*> _repnodes;
	const std::vector<RNG *> _rngs;
	unsigned int _nrep;
	unsigned int _n;
    public:
	DefaultPDMonitor(std::vector<StochasticNode const *> const &snodes,
			 std::vector<RNG *> const &rngs, unsigned int nrep); 
	~DefaultPDMonitor();
	void update();
    };

}

#endif /* DEFAULT_PD_MONITOR_H_ */
