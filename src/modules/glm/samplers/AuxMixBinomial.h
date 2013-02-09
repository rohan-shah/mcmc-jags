#ifndef AUX_MIX_BINOM_H_
#define AUX_MIX_BINOM_H_

#include "Outcome.h"

namespace jags {
namespace glm {

    class LGMix;

    /**
     * @short Finite normal mixture approximation for Binomial
     * logistic variables
     * 
     * Represents a binomial outcome with logit link in terms of an
     * underlying continuous auxiliary variable. The distribution of
     * the auxiliary variable is approximated by a finite mixture of
     * normals.
     */
    class AuxMixBinomial : public Outcome
    {
	double const &_nb; // sample size of binomial distribution
	double const &_y; // value of binomial random variable
	double _y_star; // aggregated utility
	LGMix *_mix; // normal mixture
      public:
        /**
	 * @param snode Stochastic node representing the true outcome
	 * variable of a GLM
	 * 
	 * @param chain Index number of the chain (starting from zero)
	 * to use                                                                                                                                                                      */
	AuxMixBinomial(StochasticNode const *snode, unsigned int chain);
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
	/**
	 * AuxMixBinomial represents binary or binomial outcomes with 
	 * a logistic link
	 */
	static bool canRepresent (StochasticNode const *snode);
    };

}}

#endif /* AUX_MIX_BINOM_H_ */
