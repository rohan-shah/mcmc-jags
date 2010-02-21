#ifndef CAL_KL_H_
#define CAL_KL_H_

#include <config.h>

namespace dic {
 
    class CalKL {
      public:
	virtual ~CalKL();
	virtual double divergence(unsigned int ch1, unsigned int ch2) const = 0;
    };

}

#endif /* CAL_KL_H_ */
