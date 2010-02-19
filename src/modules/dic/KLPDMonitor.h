#ifndef KL_PD_MONITOR_H_
#define KL_PD_MONITOR_H_

//#include <vector>

#include "PDMonitor.h"
#include "KL.h"

class StochasticNode;
class RNG;

namespace dic {

    class KLPDMonitor : public PDMonitor {
	std::vector<StochasticNode const *> _snodes;
	KL const *_kl;
	unsigned int _n;
    public:
	KLPDMonitor(std::vector<StochasticNode const *> const &snodes, 
		    KL const *kl);
        ~KLPDMonitor();
	void update();
    };

}

#endif /* KL_PD_MONITOR_H_ */
