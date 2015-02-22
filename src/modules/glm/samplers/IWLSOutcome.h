#ifndef IWLS_OUTCOME_H_
#define IWLS_OUTCOME_H_

#include "Outcome.h"
#include "Classify.h"

namespace jags {

class LinkNode;

namespace glm {

    /**
     * @short Outcome for GLMs with Iteratively Weighted Least Squares
     *
     * The IWLSOutcome method uses a local linear approximation as
     * used by the IWLS algorithm
     */
    class IWLSOutcome : public Outcome {
	LinkNode const * _link;
	GLMFamily _family;
	double const &_y;
	double const &_scale;
	unsigned int _chain;
    public:
	IWLSOutcome(StochasticNode const *snode, unsigned int chain);
	/**
	 * Returns the value of the outcome variable in a local linear
	 * approximation to the GLM
	 */
	double value() const;	
	/**
	 * Returns the precision of the outcome variable in a local
	 * linear approximation to the GLM
	 */
	double precision() const;
	/**
	 * Returns the variance function
	 */
        double var() const;
	/**
	 * IWLSOutcome can currently represent Poisson outcomes with log link
	 * and Bernoulli or Binomial outcomes with probit or logit link.
	 */
	static bool canRepresent(StochasticNode const *snode);
    };

}}

#endif /* IWLS_OUTCOME_H_ */
