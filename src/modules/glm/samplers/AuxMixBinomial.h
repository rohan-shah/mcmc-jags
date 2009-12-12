#ifndef AUX_MIX_BINOM_H_
#define AUX_MIX_BINOM_H_

#include "AuxMix.h"

namespace glm {

    class LGMix;

    /**
     * Represents a binomial distribution with logit link as a mixture
     * of normals.
     */
    class AuxMixBinomial : public AuxMix
    {
	double const &_eta;
	double const &_nb;
	double const &_y;
	double _y_star; /* the aggregated utility */
	LGMix *_mix; /* the normal mixture */
	int _r; /* the component indicator */
      public:
	/**
	 * Constructor. The constructor uses constant references to the
	 * three parameters that define the model. The member functions
	 * gauss_approx and update use the current value of these
	 * parameters
	 *
	 * @param eta Linear predictor for the probability of success
	 * (assuming logit link) 
	 * @param nb Sample size of binomial distribution 
	 * @param y Value of binomial random variable
	 */
	AuxMixBinomial(double const &eta, double const &nb, double const &y);
	~AuxMixBinomial();
	void update(RNG *rng);
	double value() const;
	double precision() const;
    };

}

#endif /* AUX_MIX_BINOM_H_ */
