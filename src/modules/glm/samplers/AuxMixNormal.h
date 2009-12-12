#ifndef AUX_MIX_NORMAL_H_
#define AUX_MIX_NORMAL_H_

#include "AuxMix.h"

namespace glm {

    /**
     * Normal distribution as a trivial auxiliary mixture
     */
    class AuxMixNormal : public AuxMix
    {
	double const &_tau;
	double const &_y;
      public:
	/**
	 * Constructor. 
	 *
	 * @param tau Precision of normal distribution 
	 * @param y Value of normal random variable
	 */
	AuxMixNormal(double const &tau, double const &y);
	void update(RNG *rng);
	double value() const;
	double precision() const;
    };

}

#endif /* AUX_MIX_NORMAL_H_ */
