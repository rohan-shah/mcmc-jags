#ifndef AM_METHOD_H_
#define AM_METHOD_H_

#include "GLMMethod.h"

namespace jags {
namespace glm {

    class AuxMix;

    /**
     * @short Auxiliary Mixture sampler for Poisson and Binomial GLMs
     *
     * Sampler for Poisson and logistic regression proposed by Sylvia
     * Fruhwirth-Schnatter, Rudolf Fruhwirth, Leonhard Held, Havard
     * Rue (2009) Improved auxiliary mixture sampling for hierarchical
     * models of non-Gaussian data. Statistics and Computing 19: 479-492
     *
     * The auxiliary mixture method uses data augmentation to replace
     * the discrete outcome with a latent continous auxiliary
     * variable, and then uses a finite mixture approximation to treat
     * the auxiliary variables as normal.
     */
    class AMMethod : public GLMMethod {
      public:
	AMMethod(GraphView const *view, 
		 std::vector<SingletonGraphView const *> const &sub_views,
		 std::vector<Outcome *> const &outcomes,
		 unsigned int chain);
	/**
	 * Returns "AuxiliaryMixture"
	 */
	std::string name() const;
	/**
	 * Updates the auxiliary variables and then calls updateLM
	 */
	void update(RNG *rng);
    };
    
}}

#endif /* AM_METHOD_H_ */
