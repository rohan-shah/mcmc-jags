#ifndef GLM_METHOD_H_
#define GLM_METHOD_H_

#include <sampler/MutableSampleMethod.h>

#include <string>
#include <vector>

extern "C" {
#include <cholmod.h>
}

namespace jags {

    struct RNG;
    class GraphView;
    class SingletonGraphView;

namespace glm {

    class Outcome;

    /**
     * @short Abstract class for sampling generalized linear models.
     *
     * GLMMethod provides a base class for sampling methods that work
     * on generalized linear models (GLMs).  Most of the update
     * machinery is provided by GLMMethod itself. 
     *
     * GLMMethod uses sparse matrix algebra provided by the 
     * CHOLMOD library. In the context of a hierarchical model,
     * mixed models appear identical to fixed-effect models except
     * that mixed models have a design matrix that is sparse.  The use
     * of CHOLMOD, along with auxiliary variable sampling,
     * allows us to handle both fixed and random effects in a
     * consistent way without needing to distinguish between them or
     * relying on asymptotic approximations.
     */
    class GLMMethod : public MutableSampleMethod {
    protected:
	GraphView const *_view;
	unsigned int _chain;
	std::vector<SingletonGraphView const *> _sub_views;
	std::vector<Outcome *> _outcomes;
	cholmod_sparse *_x;
	cholmod_factor *_factor; //???
	void symbolic();
	void calDesign() const;

    private:
	std::vector<bool> _fixed;
	unsigned int _length_max;
	unsigned _nz_prior;
	bool _init; //????
    public:
	/**
	 * Constructor.
	 *
	 * @param view Pointer to a GraphView object for all sampled nodes.
	 *
	 * @param sub_views Vector of pointers to SingletonGraphView
	 * objects with length equal to the number of sampled
	 * nodes. Each sub-view corresponds to a single sampled node.
	 *
	 * @param outcomes Vector of pointers to Outcome objects with length
	 * equal to the number of stochastic children of the sampled nodes.
	 * The GLMMethod objects takes ownership of each Outcome in the vector
	 * and frees the memory in the destructor.
	 * 
	 * @param chain Number of the chain (starting from 0) to which
	 * the sampling method will be applied.
	 */
	GLMMethod(GraphView const *view, 
		  std::vector<SingletonGraphView const *> const &sub_views,
		  std::vector<Outcome *> const &outcomes,
		  unsigned int chain);
	/**
	 * Virtual destructor
	 */
	virtual ~GLMMethod();
	/**
	 * Updates the regression parameters by treating the GLM as a
	 * linear model (LM).  All regression parameters are updated
	 * together in a block.
	 *
	 * In order to provide more flexibility, updateLM has an
	 * optional argument stochastic, which may be set to false for
	 * a deterministic update.
	 *
	 * @param rng Random number generator used for sampling
	 *
	 * @param stochastic Logical flag. If true then updateLM draws
	 * a sample from the posterior distribution of the regression
	 * parameters. If false then it sets the regression parameters
	 * to their posterior mean.
	 */
	void updateLM(RNG *rng, bool stochastic = true);
	/**
	 * Updates the regression parameters element-wise (i.e. with
	 * Gibbs sampling).  Although Gibbs sampling less efficient
	 * than the block-updating provided by updateLM, it is more
	 * widely applicable.  In particular, if the regression
	 * parameters have a truncated normal prior, the model is
	 * still amenable to Gibbs sampling.
	 */
	void updateLMGibbs(RNG *rng);
	/**
	 * Calculates the coefficients of the posterior distribution
	 * of the regression parameters. GLMMethod uses a canonical
	 * parametrization (b, A) such that "A" is the posterior
	 * precision of the parameters and the posterior mean "mu"
	 * solves (A %*% mu = b).
	 *
	 * @param b Dense vector such that (b = A %*% mu), where "mu"
	 * is the posterior mean and "A" is the posterior precision.
	 *
	 * @param A Posterior precision represented as a sparse matrix.
	 */
	void calCoef(double *&b, cholmod_sparse *&A);
	/**
	 * Updates auxiliary variables.  The default method does
	 * nothing.  Sampling methods that use auxiliary variables to
	 * reduce the GLM to a linear model must provide their own
	 * implementation.
	 *
	 * This function is called by updateLM.  Internally, updateLM
	 * calculates the posterior mean "mu" by solving the equation
	 * (A %*% mu = b) where "A" is the posterior precision. The
	 * same dense vector is used to hold "b" (before solving the
	 * equation) and "mu" (after solving the equation).  If
	 * updateLM is called with parameter "chol" set to false then
	 * updateAuxiliary is called before solving the equation: thus
	 * the first argument (y) should contain the canonical
	 * parameter (b).  If updateLM is called with "chol" set to
	 * true then updateAuxiliary is called after solving the
	 * equation: thus the first argument (y) should contain the
	 * posterior mean.
	 *
	 * IMPORTANT NOTE: GLMMethod uses a parameterization in which
	 * the current value of the parameters is considered the
	 * origin. The value of "y" (mu or b) may need to be adjusted
	 * for this centring by an implementation of updateAuxiliary.
	 * 
	 * @param y Dense vector which may be either the posterior
	 * mean "mu" or (A %*% mu), where "A" is the posterior
	 * precision.
	 *
	 * @param N Cholesky factorization of the posterior precision "A".
	 *
	 * @param rng Random number generator used for sampling.
	 */
	virtual void updateAuxiliary(cholmod_dense *y, cholmod_factor *N, 
				     RNG *rng);
	/**
	 * Returns false. Sampling methods inheriting from GLMMethod
	 * are not adaptive.
	 */
	bool isAdaptive() const;
	/**
	 * Does nothing, as GLMMethod is not adaptive.
	 */
	void adaptOff();
	/**
	 * Returns true, as GLMMethod is not adaptive
	 */
	bool checkAdaptation() const;
    };

}}

#endif /* GLM_METHOD_H_ */
