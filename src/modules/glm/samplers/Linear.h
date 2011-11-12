#ifndef LINEAR_H_
#define LINEAR_H_

#include "GLMMethod.h"

namespace glm {

    /**
     * Conjugate sampler for normal linear models.
     */
    class Linear : public GLMMethod {
	bool _gibbs;
    public:
	/**
	 * Constructor. See GLMMethod#GLMMethod for an explanation of the
	 * parameters
	 * 
	 * @param gibbs Boolean flag. If true then the parameters of the
	 * linear model will be updated element-wise by Gibbs sampling.
	 * If false then they will be updated by block-sampling from the
	 * joint posterior
	 */
	Linear(GraphView const *view,
	       std::vector<GraphView const *> const &sub_views,
	       unsigned int chain, bool gibbs);
	/**
	 * Returns the precision of the outcome variable with index i
	 */
	double getPrecision(unsigned int i) const;
	/**
	 * Returns the value of the outcome variable with index i
	 */
	double getValue(unsigned int i) const;
	/**
	 * Calls GLMMethod#updateLM or GLMMethod#updateLMGibbs depending
	 * on the value of the parameter "gibbs" in the constructor.
	 */
	void update(RNG *rng);
	/**
	 * Returns "Linear" or "LinearGibbs" depending on the value of the
	 * parameter "gibbs" in the constructor
	 */
	std::string name() const;
    };
    
}

#endif /* LINEAR_H_ */
