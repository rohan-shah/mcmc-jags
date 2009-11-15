#ifndef HOLMES_HELD_FACTORY_H_
#define HOLMES_HELD_FACTORY_H_

#include "BinaryFactory.h"
#include "BinaryGLM.h"

namespace glm {

    /**
     * @shortFactory object for conjugate linear model sampler
     */
    class HolmesHeldFactory : public BinaryFactory
    {
    public:
	HolmesHeldFactory();
	BinaryGLM *newBinary(Updater const *updater, 
			     std::vector<Updater const *> const &sub_updaters,
			     unsigned int chain) const;
    };

}

#endif /* HOLMES_HELD_FACTORY_H_ */
