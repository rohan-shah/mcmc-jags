#ifndef AUX_MIX_POISSON_H_
#define AUX_MIX_POISSON_H_

#include "AuxMix.h"

namespace glm {

    class LGMix;

    /**
     * Represents a Poisson distribution with log link as a mixture
     * of normals.
     */
    class AuxMixPoisson : public AuxMix
    {
	double const &_eta;
	double const &_y;
	LGMix *_mix1, *_mix2; /* the normal mixtures */
	double _tau1, _tau2; /* inter-arrival times */
      public:
	/**
	 * Constructor. The constructor uses references to the parameters
	 * of the model and the member functions gauss_approx and update
	 * use the current values of these parameters.
	 *
	 * @param eta Linear predictor for the mean (assuming log link)
	 * @param y Value of Poisson random variable
	 * @param rng Random number generator used to initialize mixture
	 */
	AuxMixPoisson(double const &eta, double const &y);
	~AuxMixPoisson();
	void update(RNG *rng);
	double value() const;
	double precision() const;
    };

}

#endif /* AUX_MIX_POISSON_H_ */
