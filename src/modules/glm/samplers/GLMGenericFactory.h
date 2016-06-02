#ifndef GLM_GENERIC_FACTORY_H_
#define GLM_GENERIC_FACTORY_H_

#include "GLMFactory.h"

namespace jags {
namespace glm {

    /**
     * @short Factory object for generic GLM
     */
    class GLMGenericFactory : public GLMFactory
    {
      public:
	GLMGenericFactory();
	/**
	 * Returns true if the outcome can be represented as a mixture
	 * of normals.
	 */
	bool checkOutcome(StochasticNode const *snode) const;
	/**
	 * Returns a newly allocated object of class GLMMethod.
	 */
	GLMMethod *
	    newMethod(GraphView const *view,
		      std::vector<SingletonGraphView const *> const &sub_views, 
		      unsigned int chain, bool gibbs) const;
    };

}}

#endif /* GLM_GENERIC_FACTORY_H_ */
