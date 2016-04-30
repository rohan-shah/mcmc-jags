#ifndef ALBERT_CHIB_H_
#define ALBERT_CHIB_H_

#include "GLMBlock.h"

namespace jags {
namespace glm {

    /**
     * @Short Albert and Chib sampler for binary GLMs
     *
     * Sampler for probit models with binary outcomes using the
     * auxiliary variable algorithm described by Albert, J. and Chib,
     * S. (1993). Bayesian analysis of binary and polychotomous
     * response data. Journal of the American Statistical Association,
     * 88:669-679.
     *
     * The algorithm is extended to logistic regression models by
     * considering a latent auxiliary variable with a logistic
     * distribution as a scale mixture of normals. This extension was
     * proposed by Holmes C and Held L (2006).  Bayesian Auxiliary
     * Variables Models for Binary and Multinomial Regression,
     * Bayesian Analysis, 1:148-168.
     *
     * A second algorithm for binary probit and logistic models is
     * implemented in the class HolmesHeld.
     */
    class AlbertChib : public GLMBlock {
    public:
	/**
	 * Constructor.
	 * 
	 * @see GLMMethod#GLMMethod
	 */
	AlbertChib(GraphView const *view, 
		   std::vector<SingletonGraphView const *> const &sub_views,
		   std::vector<Outcome *> const &outcomes,
		   unsigned int chain);
    };
    
}}

#endif /* ALBERT_CHIB_H_ */
