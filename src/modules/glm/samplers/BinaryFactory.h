#ifndef BINARY_FACTORY_H_
#define BINARY_FACTORY_H_

#include "GLMFactory.h"

namespace glm {

    class BinaryGLM;

    class BinaryFactory : public GLMFactory
    {
	bool _gibbs;
    public:
	BinaryFactory(std::string const &name, bool gibbs);
	bool checkOutcome(StochasticNode const *snode,
			  LinkNode const *lnode) const;
	GLMMethod *newMethod(GraphView const *view,
			     std::vector<GraphView const *> const &sub_views,
			     unsigned int chain) const;
	bool canSample(StochasticNode const *snode) const;
	virtual BinaryGLM *newBinary(GraphView const *view,
				     std::vector<GraphView const *> const 
				     &sub_views,
				     unsigned int chain) const = 0;
    };

}

#endif /* BINARY_FACTORY_H_ */
