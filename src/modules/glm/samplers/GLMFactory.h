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
 * graph. They differ only by the allowed link function(s) and outcome
 * distribution(s) for the glm, and the update methods that they use
 * for sampling.
 */
    class GLMFactory : public SamplerFactory
    {
	std::string _name;
	GraphView * makeView(StochasticNode *snode, Graph const &graph) const;
	bool checkDescendants(GraphView const *view) const;
    public:
	GLMFactory(std::string const &name);
	virtual ~GLMFactory();
	Sampler * makeSampler(std::set<StochasticNode*> const &nodes, 
			      Graph const &graph) const;
	std::vector<Sampler*> 
	    makeSamplers(std::set<StochasticNode*> const &nodes, 
			 Graph const &graph) const;
	virtual bool checkOutcome(StochasticNode const *snode,
				  LinkNode const *lnode) const = 0;
	virtual GLMMethod *
	    newMethod(GraphView const *view,
		      std::vector<GraphView const *> const &sub_views, 
		      unsigned int chain) const = 0;
	virtual bool canSample(StochasticNode const *snode) const = 0;
	std::string name() const;
	
    };

}

#endif /* GLM_FACTORY_H_ */
