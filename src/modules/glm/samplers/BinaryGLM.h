#ifndef BINARY_GLM_H_
#define BINARY_GLM_H_

#include "GLMMethod.h"

/** 
 * Enumeration that allows us to classify the GLMs handled by the
 * class BinaryGLM.
 */
enum BGLMOutcome {BGLM_NORMAL, BGLM_LOGIT, BGLM_PROBIT};

namespace glm {

    /**
     * @short Base class for binary GLM sampling methods.
     *
     * Methods inheriting from BinaryGLM are capable of handling
     * heterogeneous GLMs that include both normal outcomes (with
     * identity link) and binary outcomes (with probit or logit link).
     *
     * For binary outcomes, GLMMethod provides auxiliary variables
     * representing a latent normal variable (z[i]) and its precision
     * (tau[i]). These are used to by the parent GLMMethod class to
     * treat the model as a linear model. The auxiliary variables are
     * protected and are thus directly available to sub-classes.
     */
    class BinaryGLM : public GLMMethod {
    protected:
	std::vector<BGLMOutcome> _outcome; // Classify outcome and link
	std::vector<double> _z; // Latent normal variable for binary outcomes
	std::vector<double> _tau; // Precision parameter for logit link
    public:
	/**
	 * Constructor.  The outcome is classified as one of: normal
	 * outcome with identity link; binary outcome with probit link;
	 * binary outcome with logit link.  A logic error is thrown
	 * if the outcome does not fit into any of these three categories.
	 *
	 * See GLMMethod#GLMMethod for a description of the parameters
	 */
	BinaryGLM(GraphView const *view, 
		  std::vector<GraphView const *> const &sub_views,
		  unsigned int chain);
	/**
	 * Initializes the auxiliary variables.  For binary outcomes, the
	 * auxiliary variable z[i] is initialized based on the
	 * observed value: z[i] is sampled from a truncated normal
	 * distribution with mean given by GLMMethod#getMean and
	 * variance 1. The sampling truncated to I(z<0) if the outcome
	 * is 0 and I(z>=0) if the outcome is 1.
	 *
	 * Sub-classes of BinaryGLM must call this function on the
	 * first call to the update function.
	 */
	void initAuxiliary(RNG *rng);
	/**
	 * Returns the outcome of observation i.  For normal outcomes,
	 * this is the observed value. For binary outcomes, it is the
	 * value of an auxiliary variable with a normal distribution
	 * (z[i]).
	 */
	double getValue(unsigned int i) const;
	/**
	 * Returns the precision of outcome i. For gaussian outcomes,
	 * this is the variance of the observed outcome. For binary
	 * outcomes, it is an auxiliary variable representing the
	 * precision of the latent normal variable (tau[i]) returned
	 * by getValue.
	 */
	double getPrecision(unsigned int i) const;
    };
    
}

#endif /* BINARY_GLM_H_ */
