#ifndef AUX_MIX_BINOM_H_
#define AUX_MIX_BINOM_H_

#include "AuxMix.h"

namespace glm {

    class LGMix;

    /**
     * @short Finite normal mixture approximation for Binomial variables
     * 
     * Represents a binomial outcome with logit link in terms of an
     * underlying continuous auxiliary variable. The distribution of
     * the auxiliary variable is approximated by a finite mixture of
     * normals.
     */
    class AuxMixBinomial : public AuxMix
    {
	double const &_eta;
	double const &_nb;
	double const &_y;
	double _y_star; // the aggregated utility
	LGMix *_mix; // the normal mixture
	int _r; // the component indicator
      public:
	/**
	 * Constructor. The constructor uses constant references to
	 * the three parameters that define the model. The member
	 * functions update uses the current value of these parameters
	 *
	 * @param eta Linear predictor for the probability of success
	 * (assuming logit link) 
	 * @param nb Sample size of binomial distribution 
	 * @param y Value of binomial random variable
	 */
	AuxMixBinomial(double const &eta, double const &nb, double const &y);
	~AuxMixBinomial();
	/**
	 * Samples the auxiliary variable from its posterior distribution
	 * given y and calculates a new normal mixture approximation
	 */
	void update(RNG *rng);
	/**
	 * Returns the residual of the auxiliary variable according to
	 * the current normal approximation
	 */
	double value() const;
	/**
	 * Returns the precision of the auxiliary variable according to
	 * the current normal approximation
	 */
	double precision() const;
    };

}

#endif /* AUX_MIX_BINOM_H_ */
