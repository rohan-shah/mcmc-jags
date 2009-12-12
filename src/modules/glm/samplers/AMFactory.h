#ifndef AM_FACTORY_H_
#define AM_FACTORY_H_

#include "GLMFactory.h"
#include "AMMethod.h"

namespace glm {

    /**
     * @shortFactory object for conjugate linear model sampler
     */
    class AMFactory : public GLMFactory
    {
      public:
	AMFactory();
	bool checkOutcome(StochasticNode const *snode,
			  LinkNode const *lnode) const;
	bool canSample(StochasticNode const *snode) const;
	GLMMethod * newMethod(Updater const *updater,
			      std::vector<Updater const *> const &sub_updaters, 
			      unsigned int chain) const;
    };

}

#endif /* AM_FACTORY_H_ */
