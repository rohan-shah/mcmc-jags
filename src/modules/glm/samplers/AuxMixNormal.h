#ifndef AUX_MIX_NORMAL_H_
#define AUX_MIX_NORMAL_H_

#include "AuxMix.h"

namespace glm {

    /**
     * @short Normal distribution as a trivial auxiliary mixture
     *
     * In order to allow GLMs with heterogeneous outcomes, we provide
     * a trivial AuxMix wrapper for normal random variables.
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
	/**
	 * This does nothing, as there is no approximation to update.
	 */
	void update(RNG *rng);
	/**
	 * Returns the value of the normal random variable (y in the
	 * constructor)
	 */
	double value() const;
	/**
	 * Returns the precision of the normal random variable (tau
	 * in the constructor)
	 */
	double precision() const;
    };

}

#endif /* AUX_MIX_NORMAL_H_ */
