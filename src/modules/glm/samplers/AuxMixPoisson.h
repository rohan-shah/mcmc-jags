#ifndef AUX_MIX_POISSON_H_
#define AUX_MIX_POISSON_H_

#include "AuxMix.h"

namespace glm {

    class LGMix;

    /**
     * @short Finite normal mixture approximation for Poisson variables
     *
     * Represents a Poisson outcome with log link in terms of one or
     * two underlying continous auxiliary variables and then
     * approximates the distribution of the auxiliary variables as a
     * mixture of normals.
     *
     * The auxiliary variables are based on inter-arrival times of a
     * Poisson process.  If the Poisson count is zero, only one
     * auxiliary variable is required. Otherwise two are used.
     */
    class AuxMixPoisson : public AuxMix
    {
	double const &_eta;
	double const &_y;
	LGMix *_mix1, *_mix2; /* the normal mixtures */
	double _tau1, _tau2; /* inter-arrival times */
      public:
	/**
	 * Constructor. The constructor uses references to the
	 * parameters of the model and the member function update uses
	 * the current values of these parameters.
	 *
	 * @param eta Linear predictor for the mean (assuming log link)
	 * @param y Value of Poisson random variable
	 * @param rng Random number generator used to initialize mixture
	 */
	AuxMixPoisson(double const &eta, double const &y);
	~AuxMixPoisson();
	/**
	 * Samples the auxiliary variable from its posterior distribution
	 * given the outcome (y in the constructor) and then calculates
	 * a new normal approximation
	 */
	void update(RNG *rng);
	/**
	 * Returns a weighted mean of the residuals from the current
	 * normal approximation. The residuals are weighted by their
	 * precisions. This provides a single value to AMMethod, even
	 * when there are two auxiliary variables.
	 */
	double value() const;
	/**
	 * Returns the sum of the precisions of the two auxiliary variables
	 */
	double precision() const;
    };

}

#endif /* AUX_MIX_POISSON_H_ */
