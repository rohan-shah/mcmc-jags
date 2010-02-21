#ifndef CAL_KL_APPROX_H_
#define CAL_KL_APPROX_H_

#include "CalKL.h"
#include <vector>

class StochasticNode;
class RNG;

namespace dic {

    class CalKLApprox : public CalKL {
	StochasticNode *_repnode;
	const std::vector<RNG *> _rngs;
	const unsigned int _nrep;
      public:
	CalKLApprox(StochasticNode const *snode, 
		    std::vector<RNG*> const &rngs, unsigned int nrep);
	~CalKLApprox();
	double divergence(unsigned int ch1, unsigned int ch2) const;
    };

}

#endif /* CAL_KL_APPROX_H_ */
