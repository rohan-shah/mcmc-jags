#ifndef HOLMES_HELD_FACTORY_H_
#define HOLMES_HELD_FACTORY_H_

#include "GLMFactory.h"

namespace glm {

    /**
     * @shortFactory object for conjugate linear model sampler
     */
    class HolmesHeldFactory : public GLMFactory
    {
    public:
	HolmesHeldFactory();
	bool checkOutcome(StochasticNode const *snode,
			  LinkNode const *lnode) const;
	GLMMethod *newMethod(Updater const *updater, 
			     std::vector<Updater const *> const &sub_updaters,
			     unsigned int chain) const;
	bool trunc() const;
    };

}

#endif /* HOLMES_HELD_FACTORY_H_ */
