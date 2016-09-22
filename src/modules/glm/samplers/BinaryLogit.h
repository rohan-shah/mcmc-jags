#ifndef BINARY_LOGIT_H_
#define BINARY_LOGIT_H_

#include <config.h>

#include "Outcome.h"
#include <graph/StochasticNode.h>

namespace jags {

class StochasticNode;

namespace glm {

    /*
     * @short Binary outcome with logistic link
     *
     * Outcome for binary logistic regression models, based on Holmes
     * C and Held L (2006).  Bayesian Auxiliary Variables Models for
     * Binary and Multinomial Regression, Bayesian Analysis,
     * 1:148-168.
     *
     * The binary outcome is represented in terms of a latent logistic
     * distribution which, in turn, is represented as a scale mixture of
     * normals
     */
    class BinaryLogit : public Outcome
    {
	double const &_y;
	double _z;
	double _tau;
	double _sigma2;
      public:
	BinaryLogit(StochasticNode const *snode, unsigned int chain);
	double value() const;
	double precision() const;
	void update(RNG *rng);
	void update(double mean, double var, RNG *rng);
	static bool canRepresent(StochasticNode const *snode);
    };

}}

#endif /* BINARY_LOGIT_H_ */
