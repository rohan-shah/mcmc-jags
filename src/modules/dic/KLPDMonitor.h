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
	std::vector<KL const *> _kl;
    public:
	KLPDMonitor(std::vector<StochasticNode const *> const &snodes, 
		    std::vector<KL const *> const &kl);
	double divergence(unsigned int k, unsigned int ch1, unsigned int ch2) 
	    const;
	double weight(unsigned int k, unsigned int ch) const;
    };

}

#endif /* KL_PD_MONITOR_H_ */
