#ifndef AUX_MIX_POISSON_H_
#define AUX_MIX_POISSON_H_

#include "Outcome.h"

namespace jags {
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
    class AuxMixPoisson : public Outcome
    {
	double const &_y;
	LGMix *_mix1, *_mix2; // the normal mixtures
	double _tau1, _tau2;  // inter-arrival times
      public:
        /**
	 * Constructor
	 *
	 * @param snode Stochastic node representing a Poisson outcome
	 * variable of a GLM with log link.
	 * 
	 * @param chain Index number of the chain (starting from zero).
	 */
	AuxMixPoisson(StochasticNode const *snode, unsigned int chain);
	~AuxMixPoisson();
	/**
	 * Samples the auxiliary variables from their posterior
	 * distribution given the outcome and then calculates a new
	 * normal approximation
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
	/**
	 * AuxMixPoisson represents Poisson outcomes with a log link
	 */
	static bool canRepresent (StochasticNode const *snode);
    };

}}


#endif /* AUX_MIX_POISSON_H_ */
