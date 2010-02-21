#ifndef CAL_KL_EXACT_H_
#define CAL_KL_EXACT_H_

#include "CalKL.h"

class StochasticNode;

namespace dic {

    class KL;
 
    class CalKLExact : public CalKL {
	StochasticNode const *_snode;
	KL const *_kl;
      public:
	CalKLExact(StochasticNode const *snode, KL const *kl);
	double divergence(unsigned int ch1, unsigned int ch2) const;
    };

}

#endif /* CAL_KL_EXACT_H_ */
