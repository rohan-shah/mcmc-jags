#ifndef GLM_FACTORY_H_
#define GLM_FACTORY_H_

#include "GLMMethod.h"
#include <sampler/SamplerFactory.h>

class LinkNode;

namespace glm {

    /**
     * @short Abstract factory for GLM samplers 
     * 
     * All factory objects for samplers that handle generalized linear
     * models (GLMs) must recognize the same basic structures in the
     * graph. They differ only by the allowed link function(s) and
     * outcome distribution(s) for the GLM, and the update methods
     * that they use for sampling.
     */
    class GLMFactory : public SamplerFactory
    {
	std::string _name;
	GraphView * makeView(StochasticNode *snode, Graph const &graph) const;
	bool checkDescendants(GraphView const *view) const;
    public:
	GLMFactory(std::string const &name);
	virtual ~GLMFactory();
	/**
	 * Creates a newly-allocated Sampler for the set of candidate nodes,
	 * or a NULL pointer. Sub-classes of GLMFactory only have to
	 * implement the abstract member function newMethod.
	 * the 
	 */
	Sampler * makeSampler(std::set<StochasticNode*> const &nodes, 
			      Graph const &graph) const;
	/**
	 * Wraps GLMFactory#makeSampler and returns a single
	 * newly-allocated sampler in a vector.
	 */
	std::vector<Sampler*> 
	    makeSamplers(std::set<StochasticNode*> const &nodes, 
			 Graph const &graph) const;
	/**
	 * Checks that an outcome variable in a GLM has the correct
	 * family and link function.
	 *
	 * @param snode Stochastic node representing an outcome variable
	 *
	 * @param lnode LinkNode representing a link function. The identity
	 * link is represented by a NULL pointer.
	 */
	virtual bool checkOutcome(StochasticNode const *snode,
				  LinkNode const *lnode) const = 0;
	/**
	 * Function for returning a newly allocated GLMMethod.  This
	 * is called by GLMFactory#makeSampler
	 *
	 * @param view Pointer to a GraphView object for all sampled nodes.
	 *
	 * @param sub_views Vector of pointers to GraphView objects with
	 * length equal to the number of sampled nodes. Each sub-view
	 * corresponds to a single sampled node.
	 *
	 * @param chain Number of the chain (starting from 0) to which
	 * the sampling method will be applied.
	 */
	virtual GLMMethod *
	    newMethod(GraphView const *view,
		      std::vector<GraphView const *> const &sub_views, 
		      unsigned int chain) const = 0;
	/**
	 * Virtual function for checking that that a candidate
	 * stochastic node is suitable for sampling.  Currently, we
	 * assume that all candidate nodes have a normal distribution:
	 * this does not need to be checked by canSample.  However,
	 * some sampling methods have restrictions on which nodes can
	 * be sampled, e.g. they must be unbounded, or scalar. Such
	 * restrictions must be coded in the canSample function.
	 */
	virtual bool canSample(StochasticNode const *snode) const = 0;
	/**
	 * Returns the string provided as the parameter "name" in the
	 * constructor.
	 */
	std::string name() const;
	/**
	 * Returns true if the factory can only work with outcome
	 * variables that are observed. The default implementation
	 * returns false.
	 *
	 * If a sampling method uses auxiliary variables, then
	 * generally it cannot work with unobserved outcomes.  With
	 * auxiliary variable sampling the observed outcome is
	 * considered to be a deterministic function of the auxiliary
	 * variables. When an unobserved outcome variable is updated
	 * (by a sampling method independent of this one) it may be
	 * inconsistent with the current values of the auxiliary
	 * variables, leaving them in an invalid state. An exception
	 * is when the auxiliary variables can be sampled directly,
	 * without reference to the current values.
	 */
	virtual bool fixedOutcome() const;
	/**
	 * Returns true if the factory can only work on a GLM with a
	 * fixed design matrix (i.e. one that does not change between
	 * iterations) and false otherwise.  GLMs with variable design
	 * matrices occur in the context of missing data and
	 * measurement error models.  The default implementation
	 * returns false.
	 *
	 * Note that the same restrictions as for the fixedOutome
	 * function apply for auxiliary variables.
	 */
	virtual bool fixedDesign() const;
    };

}

#endif /* GLM_FACTORY_H_ */
