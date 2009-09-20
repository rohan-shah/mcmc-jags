#ifndef GLM_FACTORY_H_
#define GLM_FACTORY_H_

#include "GLMMethod.h"

#include <sampler/SamplerFactory.h>

class InverseLinkFunc;

namespace glm {

/**
 * @short Abstract factory for GLM samplers 
 *
 * All factory objects for samplers that handle generalized linear
 * models (GLMs) must recognize the same basic structures in the
 * graph. They differ only by the allowed link function(s) and outcome
 * distribution(s) for the glm, and the update methods that they use
 * for sampling.
 */
    class GLMFactory : public SamplerFactory
    {
	unsigned int canSample(StochasticNode *snode, Graph const &graph) const;
    public:
	virtual ~GLMFactory();
	Sampler * makeSampler(StochasticNodeSet const &nodes, 
			      Graph const &graph) const;
	virtual bool checkOutcome(StochasticNode const *snode) const = 0;
	virtual bool checkLink(InverseLinkFunc const *link) const = 0;
	virtual GLMMethod *newMethod() const = 0;
    };

}

#endif /* GLM_FACTORY_H_ */
